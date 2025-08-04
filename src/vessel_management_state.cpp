// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
 * [FILE NAME]
 *
 * vessel_management_state.cpp
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/22 Original by Ethan, Saviz
 * 
 * Rev 2 - 2025/??/??
 *       - Ethan.
 *       - Replaced all instanced of hard coded regex patterns with references to global
 *         variables.
 *       - Fixed various terminal text output line spacing
 *       - Changed logic for list scrolling on vessel list. No longer skips
 *         a page when scrolling to the end of the list then scrolling back. 
 *         Also immediately returns to the vessel menu when there are no vessels.
 *       - Prompts for next/previous page in the sailing report now show correct
 *         number
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  -Uses a simple `Vessel` struct for storing information related to vessels.
 *
 * Memory / speed / complexity trade‑offs:
 *
 *  -`.reserve(g_list_length)` avoids repeated vector reallocations.
 *  -DB results are pulled 5 at a time into a `std::vector<Vessel>` to avoid using too much memory.
 *
 * Input abstraction:
 *  -Regex‑based string validation (`std::regex(R"([\w ]{1,25})")`) simplifies input checks.
 *  -`continuouslyPromptFor` helpers centralize prompting, validation, and error feedback.
 *
 * Error handling and feedback:
 *  -Global flags (`g_is_successful`, `g_outcome_message`) carry DB error state.
 *
 * Debugging and logging:
 *  -Controlled by `#ifdef DEBUG_MODE` prints.
 *  -Could swap to a logging framework with levels (INFO/WARN/ERROR) for production use.
 *
 * State management:
 *  -Inherits from `State` and uses `m_state_manager->selectNextState()` to drive transitions.
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include "global.hpp"
#include "input.hpp"
#include "utilities.hpp"
#include "state_manager.hpp"
#include "vessel_management_state.hpp"
#include "database.hpp"

// ----------------------------------------------------------------------------
VesselManagementState::VesselManagementState()
{
#ifdef DEBUG_MODE
    std::cout << "[Debug] Constructor called: VesselManagementState()" << "\n";
#endif
}

// ----------------------------------------------------------------------------
VesselManagementState::~VesselManagementState()
{
}

// ----------------------------------------------------------------------------
void VesselManagementState::onEnter()
{
    std::cout <<
        "VESSEL MANAGEMENT MENU\n"
        "1) Add a new vessel\n"
        "2) List all vessels\n"
        "0) Exit to main menu\n"
        "\n";
}

// ----------------------------------------------------------------------------
void VesselManagementState::onProcess()
{
    //Container for users choice from the list of menu operations
    char user_choice = '\0';

    // Get user choice:
    continuouslyPromptForCharacter(
        "Please enter your selection [0-2]: ",
        std::vector<char>{'0', '1', '2'},
        user_choice
        );
    std::cout << "\n";

    //Execute appropriate operation and select next menu state
    switch(user_choice)
    {
        case '1':
            createVessel();
            m_state_manager->selectNextState(States::VesselManagementState);
            break;
        case '2':
            listVessels();
            m_state_manager->selectNextState(States::VesselManagementState);
            break;
        case '0':
            m_state_manager->selectNextState(States::MainMenuState);
            break;
    }
}

// ----------------------------------------------------------------------------
void VesselManagementState::onExit()
{
}

// ----------------------------------------------------------------------------
void VesselManagementState::createVessel()
{
    //Container to hold users input vessel information
    Vessel vessel;

    //Container for users [y/n] choice
    char user_choice = '\0';

    // Obtain input for new vessel:
    // ****************************************************************************

    continuouslyPromptForString(
        "Please enter the name of the new vessel: ",
        g_vessel_name_regex,
        vessel.vessel_name
        );

    continuouslyPromptForReal(
        "Please enter the high-ceiling lane length [0-1200]: ",
        0,
        1200,
        vessel.high_ceiling_lane_length
        );

    continuouslyPromptForReal(
        "Please enter the low-ceiling lane length [0-1200]: ",
        0,
        1200,
        vessel.low_ceiling_lane_length
        );

    // Confimration:
    // ****************************************************************************

    std::cout << "\n";

    continuouslyPromptForCharacter(
        "Are you sure you want to create this vessel [y/n]? ",
        g_allowed_yes_no_responses,
        user_choice
        );
    std::cout << "\n";

    // Decide what to do based on input:
    // ****************************************************************************

    int new_vessel_id = -1;

    switch(user_choice)
    {
        // Attempt vessel creation:
        case 'y':
        case 'Y':
            State::m_database->addVessel(vessel, new_vessel_id, g_is_successful, g_outcome_message);
            std::cout << g_outcome_message << "\n";
            break;
        case 'n':
        case 'N':
            std::cout << "Vessel creation operation aborted!" << "\n";
            break;
    }

    std::cout << "\n";
}

// ----------------------------------------------------------------------------
//
// Allow the offest to go one g_list_length past the ends of the list so when the user
// goes back one page, they will be at the first/last page of the list.
//
void VesselManagementState::listVessels()
{    
    int offset = 0; // Offset the starting record by the offset amount
    bool can_go_next = true; // able to scroll the list to higher offset
    bool can_go_previous = true; // able to scroll the list to lower offset

    // NOTE (SAVIZ): I am pretty sure we can save some performance if we create the vector once and reserve it once.
    std::vector<Vessel> vessels;

    vessels.reserve(g_list_length);

    // Continue listing vessels forever until the user exits:
    while(true)
    {
        //reset these each iteration
        can_go_next = true; 
        can_go_previous = true; 

        // if offset negative, do not try getVessels with negative offset
        if (offset < 0)
        {
            std::cout << "No more previous records available for displaying!" << "\n\n";
            can_go_previous = false; // dont allow furthur scrolling into negative offset
        }
        else 
        {
            m_database->getVessels(
                g_list_length,
                offset,
                vessels,
                g_is_successful,
                g_outcome_message
            );

            // Edge cases:
            // ****************************************************************************

            if (!g_is_successful)
            {
                std::cout << g_outcome_message << "\n\n";

                break; // Go back to menu.
            }

            // If there are no more records to show based on offset:
            if (vessels.empty())
            {
                if (offset == 0) // If we are at the start of the list.
                {
                    std::cout << "No records available for displaying!" << "\n\n";
                    break;
                }
                else // If we are past the end of the list.
                {
                    std::cout << "No more next records available for displaying!" << "\n\n";
                    can_go_next = false; // do not allow further scrolling into higher offset
                }
            }
            else
            {
                // Print the report
                // ****************************************************************************

                // Report title:
                std::cout << "Vessel Report" << std::string(13, ' ') << Utilities::getLocalDateAndTime() << "\n";

                // Column headers:
                std::cout
                    << " ID  "
                    << std::setw(25) << std::left << "Name" << "  "
                    << std::setw(6) << std::right << "LCLL" << "  "
                    << std::setw(6) << std::right << "HCLL" << "\n";

                // One row for each fetched vessel record
                for (const Vessel& vessel : vessels)
                {
                    std::cout
                        << std::setw(3) << std::right << vessel.vessel_id << ") " // ID column
                        << std::setw(25) << std::left << vessel.vessel_name << "  " // Name column
                        << std::setw(6) << std::right << std::fixed << std::setprecision(1) << vessel.low_ceiling_lane_length << "  "  // LCLL column
                        << std::setw(6) << std::right << std::fixed << std::setprecision(1) << vessel.high_ceiling_lane_length // HCLL column
                        << "\n";
                }
                std::cout << "\n";
            }
        }

        // Prompt for input:
        // ****************************************************************************

        std::cout <<
            "<p> >> View the previous " << std::to_string(g_list_length) << " vessels.\n" <<
            "<n> >> View the next " << std::to_string(g_list_length) << " vessels.\n" <<
            "<e> >> Exit the list.\n"
            "\n";

        char user_choice = '\0';

        continuouslyPromptForCharacter(
            "Please enter your selection [<p>, <n>, <e>]: ",
            g_allowed_navigation_responses,
            user_choice
            );

        // Decide what to do next:
        // ****************************************************************************

        bool user_wants_to_exit = false;

        switch(user_choice)
        {
        case 'p':
        case 'P':
            if (can_go_previous) 
            {
                offset -= g_list_length;
            }            
            break;
        case 'n':
        case 'N':
            if (can_go_next)
            {
                offset += g_list_length;
            }
            break;
        case 'e':
        case 'E':
            user_wants_to_exit = true;
            break;
        }

        std::cout << "\n";

        if(user_wants_to_exit)
        {
            break;
        }
    }
}

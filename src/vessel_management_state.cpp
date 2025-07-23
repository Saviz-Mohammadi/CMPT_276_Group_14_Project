// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Vessel Management State
 *
 *
 * [FILE NAME]
 *
 * vessel_management_state.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/22 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [DESIGN NOTES]
 * 
 * (Note, Ethan) the assignment document says "memory/speed/complexity trade-offs, 
 * representation choices and alternatives. Also, comments on private/static module 
 * constants, types, and variables."
 * 
 * When an operation started here is to return to the Vessels menu after it finishes, the 
 * Vessel Management State tells the State Manager to run this state next and then exits. 
 * This way OnEnter() is called again and the menu is printed again without need for an 
 * infinite loop.
 * 
 * The containers for storing vessel information and character input collected from the 
 * user could be stored in static variables declared in the scope of this .cpp file since 
 * only one vessel/user selection is operated on at one time, but were instead declared
 * inside each method requiring them to save small amout of memory when the method/module
 * goes out of scope.
 * 
*/

// ============================================================================
// ============================================================================

#include <iostream>
#include <vector>
#include <regex>
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
        std::regex(R"([\w ]{1,25})"), // Regular expression >> match 1-25 letters, numbers, digits, case insensitive
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
void VesselManagementState::listVessels()
{
    // Offset the starting record by the length amount:
    int offset = 0;

    // NOTE (SAVIZ): I am pretty sure we can save some performance if we create the vector once and reserve it once.
    std::vector<Vessel> vessels;

    vessels.reserve(g_list_length);

    // Continue listing vessels forever until the user exits:
    while(true)
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

        if(!g_is_successful)
        {
            std::cout << g_outcome_message << "\n\n";

            break; // Go back to menu.
        }

        // If there are no more records to show based on offset:
        if(vessels.empty())
        {
            if (offset < 0) 
            {
#ifdef DEBUG_MODE
                std::cout << "[Debug] list offset < 0" << "\n\n";
#endif
                break;
            }
            else if(offset == 0) // If we are at the start of the list.
            {
                std::cout << "No records available for displaying!" << "\n\n";
            }
            else // If we are at the end of the list.
            {
                // Clamp offset at the end:
                offset -= g_list_length;

                std::cout << "No more next records available for displaying!" << "\n\n";
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
                << std::setw(25) << std::left  << "Name" << "  "
                << std::setw(6)  << std::right << "LCLL" << "  "
                << std::setw(6)  << std::right << "HCLL" << "\n";

            // One row for each fetched vessel record
            for(const Vessel& vessel : vessels)
            {
                std::cout
                    << std::setw(3)  << std::right << vessel.vessel_id   << ") " // ID column
                    << std::setw(25) << std::left  << vessel.vessel_name << "  " // Name column
                    << std::setw(6)  << std::right << std::fixed << std::setprecision(1) << vessel.low_ceiling_lane_length << "  "  // LCLL column
                    << std::setw(6)  << std::right << std::fixed << std::setprecision(1) << vessel.high_ceiling_lane_length // HCLL column
                    << "\n";
            }
            std::cout << "\n";
        }

        // Prompt for input:
        // ****************************************************************************

        std::cout <<
            "<p> >> View the previous 5 vessels.\n"
            "<n> >> View the next 5 vessels.\n"
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
            offset -= g_list_length;

            // Clamp offset at bottom of vessel list:
            if(offset <= 0)
            {
                offset = 0;
            }
            break;
        case 'n':
        case 'N':
            offset += g_list_length;
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

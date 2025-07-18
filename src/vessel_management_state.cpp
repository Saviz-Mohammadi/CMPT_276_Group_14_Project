#include <iostream>
#include <vector>
#include <regex>
#include <ctime>
#include <iomanip>
#include "global.hpp"
#include "input.hpp"
#include "state_manager.hpp"
#include "vessel_management_state.hpp"
#include "database.hpp"

// ----------------------------------------------------------------------------
VesselManagementState::VesselManagementState()
{
#ifdef DEBUG_MODE
    std::cout << "Constructor called: VesselManagementState()" << "\n";
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
    char user_choice = '\0';

    // Get user choice:
    continuouslyPromptForCharacter(
        "Please enter your selection [0-2]: ",
        std::vector<char>{'0', '1', '2'},
        user_choice
        );

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
    Vessel vessel;
    char user_choice = '\0';

    // Obtain input for new vessel:
    // ****************************************************************************

    continuouslyPromptForString(
        "Please enter the name of the new vessel: ",
        std::regex(R"([\w ]{1,25})"), // Regular expression >> match 1-25 letters, numbers, digits, case insensitive
        vessel.vessel_name
        );

    continuouslyPromptForReal(
        "Please enter the \"High-Ceiling Lane Length\" (HCLL) [0-1200]: ",
        0,
        1200,
        vessel.high_ceiling_lane_length
        );

    continuouslyPromptForReal(
        "Please enter the \"Low-Ceiling Lane Length\" (LCLL) [0-1200]: ",
        0,
        1200,
        vessel.low_ceiling_lane_length
        );

    continuouslyPromptForCharacter(
        "Are you sure you want to create this vessel [y/n]? ",
        g_allowed_yes_no_responses,
        user_choice
        );

    // Decide what to do based on input:
    // ****************************************************************************

    switch(user_choice)
    {
        // Attempt vessel creation:
        case 'y':
        case 'Y':
            State::m_database->addVessel(vessel, g_is_successful, g_outcome_message);

            if(g_is_successful)
            {
                std::cout << "New vessel successfully created!" << "\n";
            }

            else
            {
                std::cout << g_outcome_message << "\n";
            }
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

    // Continue listing vessels forever until the user exits:
    while(true)
    {
        std::vector<Vessel> vessels;

        vessels.reserve(g_list_length);

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
            // NOTE (SAVIZ): I understand that the value technically cannot be less than zero, but it's good programming practice to account for such cases nonetheless:
            if(offset == 0) // If we are at the start of the list.
            {
                std::cout << "No more previous records available for displaying!" << "\n\n";
            }

            else // If we are at the end of the list.
            {
                // Clamp top:
                offset -= g_list_length;

                std::cout << "No more next records available for displaying!" << "\n\n";
            }

            continue;  // In any case, skip the printing.
        }

        // Print the list:
        // ****************************************************************************

        std::time_t time = std::time(nullptr);
        std::tm* time_ptr = std::localtime(&time);

        // Report title:
        std::cout << "Vessel Report" << std::string(18, ' ') << std::put_time(time_ptr, "%Y-%m-%d  %H:%M:%S") << "\n";

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
                << std::setw(6)  << std::right << std::fixed << std::setprecision(1) << vessel.low_ceiling_lane_length  // LCLL column
                << std::setw(6)  << std::right << std::fixed << std::setprecision(1) << vessel.high_ceiling_lane_length // HCLL column
                << "\n";
        }

        // Prompt for input:
        // ****************************************************************************

        std::cout
            << "\n\n"
            << "<p> >> View the previous 5 vessels."
            << "<n> >> View the next 5 vessels."
            << "<e> >> Exit the list."
            << "\n\n";

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

            // Clamp bottom:
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

        std::cout << "\n\n";

        if(user_wants_to_exit)
        {
            break;
        }
    }
}



// ============================================================================

#include <vector>
#include <iostream>
#include <regex>
#include <ctime>
#include <iomanip>
#include "state.hpp"
#include "vessel_management_state.hpp"
#include "state_manager.hpp"
#include "input.hpp"
#include "containers.hpp"
#include "database.hpp"
#include "global.hpp"

static Vessel s_vessel;
static int s_vessel_list_offset;
static char s_user_choice;
static bool s_is_successful;
static std::string s_outcome_message;

// ----------------------------------------------------------------------------
VesselManagementState::VesselManagementState()
{
    
}

// ----------------------------------------------------------------------------
VesselManagementState::~VesselManagementState()
{

}

// ----------------------------------------------------------------------------
void VesselManagementState::onEnter()
{
    s_vessel = Vessel();
    s_vessel_list_offset = 0;
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
    // Get user choice:
    do
    {
        promptForCharacter(
            "Please enter your selection [0-2]: ",
            std::vector<char>('0', '1', '2'),
            s_user_choice,
            s_is_successful,
            s_outcome_message
            );

        if(!s_is_successful)
        {
            std::cout << s_outcome_message << "\n\n";
        }
    } while(!s_is_successful);

    // Switch on selection and start the appropriate action:
    switch(s_user_choice)
    {
        case '1':
            createVessel();
            m_state_manager->selectNextState(States.VesselManagementState);
            break;
        case '2':
            listVessels();
            m_state_manager->selectNextState(States.VesselManagementState);
            break;
        case '0':
            m_state_manager->selectNextState(States.MainMenuState);
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
    // ****************************************************************************
    while (true) //prompt until good vessel name
    {
        promptForString(
            "Please enter the name of the new vessel: ",
            std::regex(R"[\w ]{1,25}"), //match 1-25 letters, numbers, digits, case insensitive
            s_vessel.vesselName,
            s_is_successful,
            s_outcome_message);
        std::cout << "\n";
        if (s_is_successful)
        {
            break;
        }
        else
        {
            std::cout << s_outcome_message << "\n";
        }
    }

    // ****************************************************************************
    while (true) // prompt until good hcll
    {
        promptForReal(
            "Please enter the \"High-Ceiling Lane Length\" (HCLL) [0-1200]: ",
            0, 1200,
            s_vessel.high_ceiling_lane_length,
            s_is_successful,
            s_outcome_message);
        std::cout << "\n";
        if (s_is_successful)
        {
            break;
        }
        else
        {
            std::cout << s_outcome_message << "\n";
        }
    }

    // ****************************************************************************
    while (true) // prompt until good lcll
    {
        promptForReal(
            "Please enter the \"Low-Ceiling Lane Length\" (LCLL) [0-1200]: ",
            0, 1200,
            s_vessel.low_ceiling_lane_length,
            s_is_successful,
            s_outcome_message);
        std::cout << "\n";

        if (s_is_successful)
        {
            break;
        }
        else
        {
            std::cout << s_outcome_message << "\n";
        }
    }
    std::cout << "\n";
    // ****************************************************************************
    while (true) // prompt until good confirmation
    {
        promptForCharacter(
            "Are you sure you want to create this vessel [y/n]? ",
            std::vector<char>('y', 'Y', 'n', 'N'),
            s_user_choice,
            s_is_successful,
            s_outcome_message);
        std::cout << "\n";

        if (s_is_successful)
        {
            break;
        }
        else
        {
            std::cout << s_outcome_message << "\n";
        }
    }
    std::cout << "\n";
    // ****************************************************************************
    switch (s_user_choice) {
        case 'y':
        case 'Y': //try vessel record creation
            m_database->addVessel(s_vessel, s_is_successful, s_outcome_message);
            if (s_is_successful)
            {
                std::cout << "New vessel successfully created!";
            }
            else
            {
                std::cout << s_outcome_message;
            }
            break;
        case 'n':
        case 'N':
            std::cout << "Vessel creation operation aborted!";
            break;
    }
    std::cout << "\n\n"
}

// ----------------------------------------------------------------------------
void VesselManagementState::listVessels()
{
    while (true) // list forever until the user exits
    {
        std::vector<Vessel> vessels(g_list_length);
        m_database->getVessels(g_list_length, s_vessel_list_offset, vessels, s_is_successful, s_outcome_message);
        if (s_is_successful)
        {
            // ****************************************************************************
            // format and print the vessel list
            time_t time = std::time(nullptr);
            std::cout << "Vessel Report" << std::string(17, ' ') << std::put_time(time, "%Y-%m-%d  %H:%M:%S") << "\n";
            std::cout
                << "    " << std::setw(25) << std::left << "Name" << "  "
                << std::setw(6) << std::right << "LCLL" << "  "
                << std::setw(6) << std::right << "HCLL" << "\n";
            bool found_one_vessel = false;
            for (int i = 0; i < g_list_length; i++)
            {
                Vessel v = vessels[i];
                if (v == NULL) {
                    continue;
                }
                std::cout
                    << std::setw(2) << std::right << v.vessel_id << ") "
                    << std::setw(25) << std::left << v.vessel_name << "  "
                    << std::setw(6) << std::right << std::fixed << std::setprecision(1) << v.low_ceiling_lane_length
                    << std::setw(6) << std::right << std::fixed << std::setprecision(1) << v.high_ceiling_lane_length
                    << "\n";
            }
            // ****************************************************************************
            // prompt for page scrolling or exit
            std::cout
                << "\n\n"
                << "<p> >> View the previous 5 vessels."
                << "<n> >> View the next 5 vessels."
                << "<e> >> Exit the list."
                << "\n\n";

            // ****************************************************************************
            while (true) // prompt until valid choice
            {
                promptForCharacter(
                    "Please enter your selection [<p>, <n>, <e>]: ",
                    std::vector<char>('p', 'n', 'e'),
                    s_user_choice,
                    s_is_successful,
                    s_outcome_message);
                std::cout << "\n\n";
                if (s_is_successful)
                {
                    break;
                }
                else
                {
                    std::cout << s_outcome_message << "\n\n";
                }
            }
            // ****************************************************************************
            switch (s_user_choice) // switch on choice
            {
            case 'p':
                s_vessel_list_offset = std::max(0, s_vessel_list_offset - g_list_length);
                break;
            case 'n':
                s_vessel_list_offset = s_vessel_list_offset + g_list_length;
                break;
            case 'e':
                cout << "\n\n";
                return;
                break;
            }
            cout << "\n\n"
            // ****************************************************************************
        }
        else // database read failed
        {
            std::cout << s_outcome_message << "\n\n";
            break;
        }        
    } // endwhile
}


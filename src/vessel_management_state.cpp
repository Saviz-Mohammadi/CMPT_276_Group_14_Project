
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

// static container for storing vessel info when creating a vessel
static Vessel s_vessel;

// static container for storing the users single character responses
static char s_user_choice;

static void printVesselList(std::vector<Vessel>& vessels);

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
            std::vector<char>{'0', '1', '2'},
            s_user_choice,
            g_is_successful,
            g_outcome_message
            );

        if(!g_is_successful)
        {
            std::cout << g_outcome_message << "\n\n";
        }
    } while(!g_is_successful);

    // NOTE (SAVIZ): Here is some code to help calling 'getVessels()':

    // std::vector<Vessel> vessels;
    // database->getVessels(5, 0, vessels, is_successful, outcome_message);

    // if(!is_successful)
    // {
    //     std::cout << outcome_message << std::endl;
    // }

    // for(const Vessel& vessel : vessels)
    // {
    //     std::cout << vessel.vessel_id << ", " << vessel.vessel_name << ", " << vessel.low_ceiling_lane_length << ", " << vessel.high_ceiling_lane_length << std::endl;
    // }

    // Switch on selection and start the appropriate action:
    switch(s_user_choice)
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
    // ****************************************************************************
    while (true) //prompt until good vessel name
    {
        promptForString(
            "Please enter the name of the new vessel: ",
            std::regex(R"([\w ]{1,25})"), //match 1-25 letters, numbers, digits, case insensitive
            s_vessel.vessel_name,
            g_is_successful,
            g_outcome_message);
        std::cout << "\n";
        if (g_is_successful)
        {
            break;
        }
        else
        {
            std::cout << g_outcome_message << "\n";
        }
    }

    // ****************************************************************************
    while (true) // prompt until good hcll
    {
        promptForReal(
            "Please enter the \"High-Ceiling Lane Length\" (HCLL) [0-1200]: ",
            0, 1200,
            s_vessel.high_ceiling_lane_length,
            g_is_successful,
            g_outcome_message);
        std::cout << "\n";
        if (g_is_successful)
        {
            break;
        }
        else
        {
            std::cout << g_outcome_message << "\n";
        }
    }

    // ****************************************************************************
    while (true) // prompt until good lcll
    {
        promptForReal(
            "Please enter the \"Low-Ceiling Lane Length\" (LCLL) [0-1200]: ",
            0, 1200,
            s_vessel.low_ceiling_lane_length,
            g_is_successful,
            g_outcome_message);
        std::cout << "\n";

        if (g_is_successful)
        {
            break;
        }
        else
        {
            std::cout << g_outcome_message << "\n";
        }
    }
    std::cout << "\n";
    // ****************************************************************************
    while (true) // prompt until good confirmation
    {
        promptForCharacter(
            "Are you sure you want to create this vessel [y/n]? ",
            std::vector<char>{'y', 'Y', 'n', 'N'},
            s_user_choice,
            g_is_successful,
            g_outcome_message);
        std::cout << "\n";

        if (g_is_successful)
        {
            break;
        }
        else
        {
            std::cout << g_outcome_message << "\n";
        }
    }
    std::cout << "\n";
    // ****************************************************************************
    switch (s_user_choice) {
        case 'y':
        case 'Y': //try vessel record creation
            m_database->addVessel(s_vessel, g_is_successful, g_outcome_message);
            if (g_is_successful)
            {
                std::cout << "New vessel successfully created!";
            }
            else
            {
                std::cout << g_outcome_message;
            }
            break;
        case 'n':
        case 'N':
            std::cout << "Vessel creation operation aborted!";
            break;
    }
    std::cout << "\n\n";
}

// ----------------------------------------------------------------------------
void VesselManagementState::listVessels()
{
    int highest_offet = -1;
    int vessel_list_offset = 0;
    while (true) // list forever until the user exits
    {
        std::vector<Vessel> vessels(g_list_length);
        m_database->getVessels(g_list_length, vessel_list_offset, vessels, g_is_successful, g_outcome_message);

        // ****************************************************************************
        // edge cases
        if (!g_is_successful)
        {
            std::cout << g_outcome_message << "\n\n";
            return; // back to menu
        }
        if (vessels.size() == 0) //if user has scrolled off the edge of the list
        {
            if (vessel_list_offset == 0) // if offset is zero and no records were returned then the database is empty
            {
                std::cout << "No records available for displaying!\n\n";
                return; //back to menu
            }
            else // must be at the end of the list so wrap to beginning
            {
                highest_offet = std::max(vessel_list_offset-g_list_length, highest_offet)-g_list_length; // remember where the end of the list is so we can wrap the other direction
                vessel_list_offset = 0;
                continue; // re-fetch records with the new offset
            }
        }
        // ****************************************************************************

        printVesselList(vessels);

        // prompt for page scrolling or exit
        std::cout
            << "\n\n"
            << "<p> >> View the previous 5 vessels."
            << "<n> >> View the next 5 vessels."
            << "<e> >> Exit the list."
            << "\n\n";

        while (true) // prompt until valid choice from the prompt
        {
            promptForCharacter(
                "Please enter your selection [<p>, <n>, <e>]: ",
                std::vector<char>{'p', 'n', 'e'},
                s_user_choice,
                g_is_successful,
                g_outcome_message);
            std::cout << "\n\n";
            if (g_is_successful)
            {
                break;
            }
            else
            {
                std::cout << g_outcome_message << "\n\n";
            }
        }

        switch (s_user_choice) //change offset for the list according to choice, or return to menu
        {
        case 'p':
            vessel_list_offset = vessel_list_offset - g_list_length;
            if (vessel_list_offset < 0) //we've scrolled off the beginning of the list
            {
                if (highest_offet < 0) // we haven't seen the end of the list yet so just stay here
                {
                    vessel_list_offset = 0;
                }
                else // we know where the end of the list is, wrap around and go there
                {
                    vessel_list_offset = highest_offet;
                }
            }
            break;
        case 'n':
            vessel_list_offset = vessel_list_offset + g_list_length;
            break;
        case 'e':
            std::cout << "\n\n";
            return; // back to menu
            break;
        }
        std::cout << "\n\n";
    } // endwhile
}

// ----------------------------------------------------------------------------
static void printVesselList(std::vector<Vessel>& vessels)
{
    time_t time = std::time(nullptr);
    std::tm* time_ptr = std::localtime(&time);

    //report title
    std::cout << "Vessel Report" << std::string(18, ' ') << std::put_time(time_ptr, "%Y-%m-%d  %H:%M:%S") << "\n";

    // column headers
    std::cout
        << " ID  "
        << std::setw(25) << std::left << "Name" << "  "
        << std::setw(6) << std::right << "LCLL" << "  "
        << std::setw(6) << std::right << "HCLL" << "\n";

    //one row for each fetched vessel record
    for (const Vessel& v : vessels)
    {
        std::cout
            << std::setw(3) << std::right << v.vessel_id << ") " //ID column
            << std::setw(25) << std::left << v.vessel_name << "  " //Name column
            << std::setw(6) << std::right << std::fixed << std::setprecision(1) << v.low_ceiling_lane_length //lcll column
            << std::setw(6) << std::right << std::fixed << std::setprecision(1) << v.high_ceiling_lane_length //hcll column
            << "\n";
    }
}

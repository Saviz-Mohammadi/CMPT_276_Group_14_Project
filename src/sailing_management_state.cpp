#include <vector>
#include <limits> 
#include <iostream>
#include <regex>
#include <ctime>
#include <iomanip>
#include <string> 
#include <cctype> 
#include "state.hpp"
#include "sailing_management_state.hpp"
#include "state_manager.hpp"
#include "input.hpp"
#include "containers.hpp"
#include "database.hpp"
#include "global.hpp"
#include "utilities.hpp"

// ----------------------------------------------------------------------------
SailingManagementState::SailingManagementState()
{

}

// ----------------------------------------------------------------------------
SailingManagementState::~SailingManagementState()
{

}

// ----------------------------------------------------------------------------
void SailingManagementState::onEnter()
{
    std::cout <<
        "SAILING MANAGEMENT MENU\n"
        "1) Create a new sailing\n"
        "2) Delete a sailing\n"
        "3) List all sailings\n"
        "4) View specific sailing\n"
        "0) Exit to main menu\n"
        "\n";
}

// ----------------------------------------------------------------------------
void SailingManagementState::onProcess()
{   
    // Variables
    std::string prompt = "Please enter your selection [0-4]: ";
    char user_choice = '\0'; 


    // Get user choice:
    continuouslyPromptForCharacter(prompt, std::vector<char>{'0', '1', '2', '3', '4'}, user_choice);


    // Switch on selection and start the appropriate action:
    switch (user_choice)
    {
    case '1':
        createSailing();
        break;
    case '2':
        deleteSailing();
        break;
    case '3':
        listSailingReports();
        break;
    case '4':
        listSailingReport();
        break;
    case '0':
        m_state_manager->selectNextState(States::MainMenuState);
        break;
    }
}

// ----------------------------------------------------------------------------
void SailingManagementState::onExit()
{

}

// ----------------------------------------------------------------------------
/* 
Note (Henry) 1: 
Duplicates or database errors will be checked by the database functions AFTER we have collected all parameters, 
will be annoying for the user if they input something wrong then have to input all 4 again, alternative option is to call get vessel/sailing by ID in here 
first to check if it exists but working on the assumption that we WON'T call those functions for sake of efficiency for the developer 

Note (Henry) 2: After the overloaded promptForString was added use the regex pattern std::regex(R"([A-Z]{3}-\d{2}-\d{2})") to force a TTT-dd-hh pattern by the user
Now the input modules should entirely be responsible for bad input

Note (Henry) 3: There should be possible solutions to prompt the user again after bad input like existing sailing or nonexistant vessel, they are commentented out.
Uncomment them and replace it with them if you want to 
*/ 
void SailingManagementState::createSailing()
{
    // Variables for prompting    
    std::string sailing_id_str; 
    char user_choice = '0'; 

    // Sailing varbiables
    int sailing_id_int;
    int vessel_id;
    std::string departure_terminal;
    int departure_day;
    int departure_hour;

    Sailing new_sailing; 
    Vessel referred_vessel; 
    vessel_id = -1; 
    bool valid_vessel = false; 
    bool valid_sailing = false; 
    // Get Vessel ID to create


    do {
        vessel_id = -1; 
        continuouslyPromptForInteger(
        "Please enter the ID of the vessel for the sailing: ", 
        0, 
        std::numeric_limits<int>::max(), 
        vessel_id
    );
        m_database->getVesselByID(vessel_id, referred_vessel, g_is_successful, g_outcome_message); 
        if(!g_is_successful) {
            std::cout << "Invalid Vessel ID, please try again"; 
            valid_vessel = false; 
        } 
        else if (g_is_successful == true) {
            valid_vessel = true; 
        }
        
    } while(valid_vessel = false); 

    // Get Sailing ID to create


    do {
        promptForString(
            "Please enter the ID of the sailing [TTT-dd-hh]: ",
            std::regex(R"([A-Z]{3}-\d{2}-\d{2})"), //TTT-dd-hh pattern
            sailing_id_str, 
            g_is_successful, 
            g_outcome_message
        );

        Utilities::extractSailingID(sailing_id_str, departure_terminal, departure_day, departure_hour);
        m_database->getSailingByID( departure_terminal, departure_day, departure_hour, new_sailing, g_is_successful, g_outcome_message);
        if(g_is_successful) {
            valid_sailing = false; 
            std::cout << "Sailing ID already exists"; 
        } else {
            valid_sailing = true; 
        }
    } while(valid_sailing == false); 


    // Prompting confirmation of sailing creation

    continuouslyPromptForCharacter(
        "Are you sure you want to create this new sailing [y/n]? ",
        g_allowed_yes_no_responses,
        user_choice
    ); 

    Sailing new_sailing; 
    switch(user_choice)
    {
        // Attempt Sailing creationg
        case 'y':
        case 'Y':
            Utilities::extractSailingID(sailing_id_str, departure_terminal, departure_day, departure_hour);

            // Extracting HCL and LCL from vessel
            m_database->getVesselByID(vessel_id, referred_vessel, g_is_successful, g_outcome_message); 
            double lcl = referred_vessel.low_ceiling_lane_length;
            double hcl = referred_vessel.high_ceiling_lane_length;

            new_sailing = Sailing(0, vessel_id, departure_terminal, departure_day, departure_hour, lcl, hcl); 

            m_database->addSailing(new_sailing, g_is_successful, g_outcome_message);
        

            if(g_is_successful)
            {
                std::cout << "Sailing created with ID of " << sailing_id_str << std::endl;
            }

            else
            {
                std::cout << g_outcome_message << "\n";
            }
            break;
        case 'n':
        case 'N':
            std::cout << "Canceled sailing creation" << std::endl;
            break;
    }

   

}

// ----------------------------------------------------------------------------
void SailingManagementState::deleteSailing()
{
    // Variables for prompting    
    std::string sailing_id_str; 
    char user_choice = '0'; 
    int sailing_id_int;
    std::string departure_terminal;
    int departure_day;
    int departure_hour;


    // Get Sailing ID to delete

    promptForString(
        "Please enter the ID of the sailing [TTT-dd-hh]: ", 
        sailing_id_str, 
        g_is_successful, 
        g_outcome_message
    );

    // Prompting confirmation of sailing deletion

    continuouslyPromptForCharacter(
        "Are you sure you want to delete this sailing [y/n]? ", 
        g_allowed_yes_no_responses, 
        user_choice 
    ); 

    Sailing referred_sailing; 
    switch(user_choice)
    {
        // Attempt vessel deletion:
        case 'y':
        case 'Y':
            Utilities::extractSailingID(sailing_id_str, departure_terminal, departure_day, departure_hour); 
            m_database->getSailingByID(departure_terminal, departure_day, departure_hour, referred_sailing, g_is_successful, g_outcome_message); 
            m_database->removeSailing(referred_sailing, g_is_successful, g_outcome_message); 

            if(g_is_successful)
            {
                std::cout << "Sailing successfully deleted!" << "\n";
            }

            else
            {
                std::cout << g_outcome_message << "\n";
            }
            break;
        case 'n':
        case 'N':
            std::cout << "Sailing deletion operation aborted!" << "\n";
            break;
    }
}

// ----------------------------------------------------------------------------
void SailingManagementState::listSailingReports()
{   
    // Checker
    bool stop = false; 
    // Variables for prompting
    std::string prompt; 
    std::string output ;

    // Sailing variables
    std::vector<SailingReport> sailing_reports; 

    // Display variables
    int current = 1;
    int offset = 0; 


    // Beginning display
    std::cout << "Sailing Report                                                 " << "YYYY-MM-DD" << "  " << "HH:MM:SS\n"; // Replace placeholders with variables of time later
    std::cout << "    Sailing ID  Vessel Name                       LCLR    HCLR  Vehicles  \%Occupied\n"; 
    


    do {
        m_database->getSailingReports(g_list_length, offset, sailing_reports, g_is_successful, g_outcome_message);
        for (const SailingReport& report : sailing_reports) {
            std::string sailing_id_str;
            Utilities::createSailingID(report.sailing.departure_terminal, report.sailing.departure_day, report.sailing.departure_hour, sailing_id_str);
            std::cout
                << std::setw(2) << std::right << current << ") "
                << std::setw(10) << std::left << sailing_id_str << " "
                << std::setw(30) << std::left << report.vessel.vessel_name
                << std::fixed << std::setprecision(1)
                << std::setw(7) << std::right << report.sailing.low_remaining_length // Clamp to >= 0 here
                << std::setw(7) << std::right << report.sailing.high_remaining_length
                << std::setw(9) << std::right << report.vehicle_count
                << std::setw(10) << std::right << report.occupancy_percentage << "%"
                << "\n";
            ++current; 
        }

        std::cout
            << "\n\n"
            << "<p> >> View the previous 5 sailings."
            << "<n> >> View the next 5 sailings."
            << "<e> >> Exit the list."
            << "\n\n";

        char user_choice = '\0';

        continuouslyPromptForCharacter(
            "Please enter your selection [<p>, <n>, <e>]: ",
            g_allowed_navigation_responses,
            user_choice
        );

        if(std::tolower(user_choice) == 'e') {
            stop = true; 
            break; 
        } 
        else if (std::tolower(user_choice) == 'n') {
            stop = false; 
            offset += g_list_length; 
        }
        else if (std::tolower(user_choice) == 'p') {    
            stop = false; 
            offset = (offset >= g_list_length) ? (offset - g_list_length) : 0;  // Lower bound of 0
             
        }
    } while(stop == false); 

}

// ----------------------------------------------------------------------------
void SailingManagementState::listSailingReport()
{
    // Variables for prompting    
    std::string sailing_id_str; 
    char user_choice = '0'; 

    // Variables to store sailing data
    int sailing_id_int;
    std::string departure_terminal;
    int departure_day;
    int departure_hour;


    // Get Sailing ID to display

    promptForString(
        "Please enter the ID of the sailing [TTT-dd-hh]: ", 
        sailing_id_str, 
        g_is_successful, 
        g_outcome_message
    );


    Sailing referred_sailing;
    SailingReport report; 
    Utilities::extractSailingID(sailing_id_str, departure_terminal, departure_day, departure_hour); 
    m_database->getSailingByID(departure_terminal, departure_day, departure_hour, referred_sailing, g_is_successful, g_outcome_message); 
    m_database->getSailingReportByID(referred_sailing, report, g_is_successful, g_outcome_message); 

   
    std::cout << " Sailing ID  Vessel Name                       LCLR    HCLR  Vehicles  \%Occupied\n"; 
    std::cout
        << " " 
        << std::setw(10) << std::left << sailing_id_str << " "
        << std::setw(30) << std::left << report.vessel.vessel_name
        << std::fixed << std::setprecision(1)
        << std::setw(7) << std::right << report.sailing.low_remaining_length
        << std::setw(7) << std::right << report.sailing.high_remaining_length
        << std::setw(9) << std::right << report.vehicle_count
        << std::setw(10) << std::right << report.occupancy_percentage << "%"
        << "\n";
        
}
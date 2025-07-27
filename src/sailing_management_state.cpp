// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*
 * [MODULE]
 *
 * Sailing Management State
 *
 *
 * [FILE NAME]
 *
 * sailing_management_state.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 – 2025/07/23 Original by Saviz Mohammadi, Henry Nguyen, Ethan Scott
 *
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  - Uses a `Sailing` struct for create/delete operations.
 *  - Uses `SailingReport` for listing and detailed view of sailings.
 *
 * Memory / speed / complexity trade‑offs:
 *  - Reserves `std::vector<SailingReport>` to `g_list_length` to minimize reallocations.
 *  - Validation loops may incur repeated database calls; consider batch checks or caching.
 *
 * Input abstraction:
 *  - Regex‑based prompts (`std::regex(R"([A-Z]{3}-\d{2}-\d{2})")`) enforce ID formatting.
 *  - `continuouslyPromptFor...` helpers centralize user input, validation, and error feedback.
 *
 * Error handling & feedback:
 *  - Uses global flags (`g_is_successful`, `g_outcome_message`) to capture DB errors.
 *  - Re‑prompts user on invalid vessel or sailing IDs.
 *
 * Debugging & logging:
 *  - `#ifdef DEBUG_MODE` can wrap additional diagnostics.
 *
 * State management:
 *  - Inherits from `State`; uses `m_state_manager->selectNextState()` for transitions.
 *
 * Future enhancements:
 *  - Extract common DB‑validation loops into shared utility functions.
 *  - Improve UX by offering abort vs retry choices explicitly.
 *  - Implement full pagination display for sailing reports in `listSailingReports()`.
 */

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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
    // Get input:
    char user_choice = '\0'; 

    continuouslyPromptForCharacter(
        "Please enter your selection [0-4]: ",
        std::vector<char>{'0', '1', '2', '3', '4'},
        user_choice
        );


    // Decide the appropriate action based on input:
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
    // Get vessel from user:
    // ****************************************************************************

    // TODO (SAVIZ): One question: Should we abort the operation and let them start over if they fail to select a correct existing Vessel? Or just continue asking them for it even if they get stuck?

    int vessel_id = -1;
    Vessel found_vessel;

    do {
        continuouslyPromptForInteger(
            "Please enter the ID of the vessel for the sailing: ",
            0,
            std::numeric_limits<int>::max(),
            vessel_id
            );

        m_database->getVesselByID(
            vessel_id,
            found_vessel,
            g_is_successful,
            g_outcome_message
            );

        if(!g_is_successful)
        {
            std::cout << "Invalid Vessel ID, please try again: " << g_outcome_message << "\n\n";
        }
    }while(!g_is_successful);


    // Check if sailing already exists:
    // ****************************************************************************

    // TODO (SAVIZ): Same here: Should we abort the operation and let them start over if they select a sailing that already exists?

    std::string sailing_id_string = "";
    std::string departure_terminal = "";
    int departure_day = 0;
    int departure_hour = 0;    
    Sailing found_sailing;

    do {
        continuouslyPromptForString(
            "Please enter the ID of the sailing [TTT-dd-hh]: ",
            std::regex(R"([A-Z]{3}-\d{2}-\d{2})"),
            sailing_id_string
        );

        Utilities::extractSailingID(
            sailing_id_string,
            departure_terminal,
            departure_day,
            departure_hour
            );

        m_database->getSailingByID(
            departure_terminal,
            departure_day,
            departure_hour,
            found_sailing,
            g_is_successful,
            g_outcome_message
            );

        if(g_is_successful)
        {
            std::cout << "Sailing ID already exists." << "\n\n";
        }
    }while(g_is_successful);

    Sailing new_sailing(-1, vessel_id, departure_terminal, departure_day, departure_hour, found_vessel.low_ceiling_lane_length, found_vessel.high_ceiling_lane_length);

    // Prompting for confirmation and creating the sailing:
    // ****************************************************************************

    char user_choice = '0';

    continuouslyPromptForCharacter(
        "Are you sure you want to create this new sailing [y/n]? ",
        g_allowed_yes_no_responses,
        user_choice
    );

    switch(user_choice)
    {
        // Create sailing if 'yes' is selected:
        case 'y':
        case 'Y':
        {}

            m_database->addSailing(
                new_sailing,
                g_is_successful,
                g_outcome_message
                );

            // In case of success of failure, just display the message that the database produces:
            std::cout << g_outcome_message << std::endl;
            break;

        // Abort the creation if 'no' is selected:
        case 'n':
        case 'N':
            std::cout << "Canceled sailing creation" << std::endl;
            break;
    }
}

// ----------------------------------------------------------------------------
void SailingManagementState::deleteSailing()
{
    // Get Sailing by ID:
    // ****************************************************************************

    // TODO (SAVIZ): Same here: Should we abort the operation and let them start over if they select a sailing that does not exist?

    std::string sailing_id_string = "";
    std::string departure_terminal = "";
    int departure_day = 0;
    int departure_hour = 0;
    Sailing sailing_targeted_for_deletion;

    do {
        continuouslyPromptForString(
            "Please enter the ID of the sailing [TTT-dd-hh]: ",
            std::regex(R"([A-Z]{3}-\d{2}-\d{2})"),
            sailing_id_string
            );

        Utilities::extractSailingID(
            sailing_id_string,
            departure_terminal,
            departure_day,
            departure_hour
            );

        m_database->getSailingByID(
            departure_terminal,
            departure_day,
            departure_hour,
            sailing_targeted_for_deletion,
            g_is_successful,
            g_outcome_message
            );

        if(!g_is_successful)
        {
            std::cout << g_outcome_message << "\n\n";
        }
    }while(!g_is_successful);


    // Prompting for confirmation and deleting the sailing:
    // ****************************************************************************

    char user_choice = '0';

    continuouslyPromptForCharacter(
        "Are you sure you want to delete this sailing [y/n]? ",
        g_allowed_yes_no_responses,
        user_choice
        );

    switch(user_choice)
    {
        // Delete sailing if 'yes' is selected:
        case 'y':
        case 'Y':
            m_database->removeSailing(
                sailing_targeted_for_deletion,
                g_is_successful,
                g_outcome_message
                );

            // In case of success of failure, just display the message that the database produces:
            std::cout << g_outcome_message << "\n";
            break;
        // Abort the creation if 'no' is selected:
        case 'n':
        case 'N':
            std::cout << "Sailing deletion operation aborted!" << "\n";
            break;
    }
}

// ----------------------------------------------------------------------------
void SailingManagementState::listSailingReports()
{
    // Offset the starting record by the length amount:
    int offset = 0;

    // NOTE (SAVIZ): I am pretty sure we can save some performance if we create the vector once and reserve it once.
    std::vector<SailingReport> sailing_reports;

    sailing_reports.reserve(g_list_length);

    // Continue listing sailing reports forever until the user exits:
    while(true)
    {
        m_database->getSailingReports(
            g_list_length,
            offset,
            sailing_reports,
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
        if(sailing_reports.empty())
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
                // Clamp top:
                offset -= g_list_length;

                std::cout << "No more next records available for displaying!" << "\n\n";
            }
        }

        else
        {
            // Print the report
            // ****************************************************************************

            // TODO (SAVIZ): All you have to do is to replace the header and the output with how the sailing report should look like:
            //
            //
            // << std::setw(10) << std::left << sailing_id_str << " "
            // << std::setw(30) << std::left << report.vessel.vessel_name
            // << std::fixed << std::setprecision(1)
            // << std::setw(7) << std::right << report.sailing.low_remaining_length
            // << std::setw(7) << std::right << report.sailing.high_remaining_length
            // << std::setw(9) << std::right << report.vehicle_count
            // << std::setw(10) << std::right << report.occupancy_percentage << "%";

            // Report title:
            std::cout << "Sailing Report" << std::string(13, ' ') << Utilities::getLocalDateAndTime() << "\n";

            // Column headers:
            std::cout
                << " ID  "
                << std::setw(25) << std::left  << "Name" << "  "
                << std::setw(6)  << std::right << "LCLL" << "  "
                << std::setw(6)  << std::right << "HCLL" << "\n";

            // One row for each fetched sailing report record
            for(const SailingReport& sailing_report : sailing_reports)
            {
                std::string sailing_id = "";

                // Squish the ID togther:
                Utilities::createSailingID(
                    sailing_report.sailing.departure_terminal,
                    sailing_report.sailing.departure_day,
                    sailing_report.sailing.departure_hour,
                    sailing_id
                    );

                // Print the sailing:
                 std::cout
                     << std::setw(3)  << std::right << sailing_id   << ") " // ID column
                     << std::setw(25) << std::left  << sailing_report.vessel.vessel_name << "  " // Name column
                     << std::setw(6)  << std::right << std::fixed << std::setprecision(1) << sailing_report.vessel.low_ceiling_lane_length << "  "  // LCLL column
                     << std::setw(6)  << std::right << std::fixed << std::setprecision(1) << sailing_report.vessel.high_ceiling_lane_length // HCLL column
                     << "\n";
            }
            std::cout << "\n";
        }

        // Prompt for input:
        // ****************************************************************************

        std::cout <<
            "<p> >> View the previous 5 sailings.\n"
            "<n> >> View the next 5 sailings.\n"
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

        std::cout << "\n";

        if(user_wants_to_exit)
        {
            break;
        }
    }
}

// ----------------------------------------------------------------------------
void SailingManagementState::listSailingReport()
{
    // Get Sailing by ID:
    // ****************************************************************************

    // TODO (SAVIZ): Same here: Should we abort the operation and let them start over if they select a sailing that does not exist?

    std::string sailing_id_string = "";
    std::string departure_terminal = "";
    int departure_day = 0;
    int departure_hour = 0;
    Sailing sailing_targeted_for_reporting;

    continuouslyPromptForString(
        "Please enter the ID of the sailing [TTT-dd-hh]: ",
        std::regex(R"([A-Z]{3}-\d{2}-\d{2})"),
        sailing_id_string
        );

    Utilities::extractSailingID(
        sailing_id_string,
        departure_terminal,
        departure_day,
        departure_hour
        );

    m_database->getSailingByID(
        departure_terminal,
        departure_day,
        departure_hour,
        sailing_targeted_for_reporting,
        g_is_successful,
        g_outcome_message
        );

    if(!g_is_successful)
    {
        std::cout << g_outcome_message << "\n\n";

        return; // Abort the operation.
    }

    // Get Sailing report and print it:
    // ****************************************************************************

    SailingReport sailing_report;

    m_database->getSailingReportByID(
        sailing_targeted_for_reporting,
        sailing_report,
        g_is_successful,
        g_outcome_message
        );

    std::string sailing_id = "";

    // Squish the ID togther:
    Utilities::createSailingID(
        sailing_report.sailing.departure_terminal,
        sailing_report.sailing.departure_day,
        sailing_report.sailing.departure_hour,
        sailing_id
        );


    // TODO (SAVIZ): You can print the same output from the last method here:

    // std::cout << " Sailing ID  Vessel Name                       LCLR    HCLR  Vehicles  \%Occupied\n";
    // std::cout
    // << " "
    // << std::setw(10) << std::left << sailing_id_str << " "
    // << std::setw(30) << std::left << report.vessel.vessel_name
    // << std::fixed << std::setprecision(1)
    // << std::setw(7) << std::right << report.sailing.low_remaining_length
    // << std::setw(7) << std::right << report.sailing.high_remaining_length
    // << std::setw(9) << std::right << report.vehicle_count
    // << std::setw(10) << std::right << report.occupancy_percentage << "%"
    // << "\n";

}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
 * [FILE NAME]
 *
 * reservation_management_state.cpp
 *
 * [REVISION HISTORY]
 *
 * Rev 1 – 2025/07/23 Original by Karan, Saviz
 * 
 * Rev 2 - 2025/??/??
 *       - Ethan.
 *       - Replaced all instances of hard coded regex patterns, and hard coded vehicle
 *         dimensions limits with references to global variables
 *       - Fixed terminal text output spacing
 *       - Fixed call to database::getVehicleByID() not using the user inputted 
 *         license plate
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  - Uses `Sailing` and `Vehicle` structs for reservation operations.
 *
 * Memory / speed / complexity trade‑offs:
 *  - Validation loops incur extra database calls but ensure correct input.
 *  - Only small objects and strings are held at any time, minimizing memory usage.
 *
 * Input abstraction:
 *  - Regex patterns enforce sailing‑ID and license‑plate formats.
 *  - `continuouslyPromptFor...` helpers unify prompting, validation, and error feedback.
 *
 * Error handling & feedback:
 *  - Uses global flags (`g_is_successful`, `g_outcome_message`) to capture and display DB errors.
 *  - Early `return` on failure aborts the current operation cleanly.
 *
 * Debugging & logging:
 *  - Could wrap additional diagnostics in `#ifdef DEBUG_MODE` blocks if needed.
 *
 * State management:
 *  - Inherits from `State`; uses `m_state_manager->selectNextState()` to loop or return to main menu.
 */

#include <vector>
#include <iostream>
#include "state.hpp"
#include "reservation_management_state.hpp"
#include "state_manager.hpp"
#include "input.hpp"
#include "containers.hpp"
#include "database.hpp"
#include "global.hpp"
#include "utilities.hpp"

// ****************************************************************************
ReservationManagementState::ReservationManagementState()
{
}

// ****************************************************************************
ReservationManagementState::~ReservationManagementState()
{
}

// ****************************************************************************
void ReservationManagementState::onEnter()
{
    std::cout <<
        "RESERVATION MANAGEMENT MENU\n"
        "1) Make a new reservation\n"
        "2) Cancel a reservation\n"
        "0) Exit to main menu\n"
        "\n";
}

// ****************************************************************************
void ReservationManagementState::onProcess()
{
    char user_choice = '\0';

    // Get user choice:
    continuouslyPromptForCharacter(
        "Please enter your selection [0-2]: ",
        std::vector<char>{'0', '1', '2'},
        user_choice
        );

    std::cout << "\n";

    // Decide on appropriate action:
    switch(user_choice)
    {
        case '1':
            createReservation();
            m_state_manager->selectNextState(States::ReservationManagementState);
        break;
        case '2':
            deleteReservation();
            m_state_manager->selectNextState(States::ReservationManagementState);
        break;
        case '0':
            m_state_manager->selectNextState(States::MainMenuState);
        break;
    }
}

// ****************************************************************************
void ReservationManagementState::onExit()
{  
}

// ****************************************************************************
void ReservationManagementState::createReservation()
{
    // Get Sailing ID:
    // ----------------------------------------------------------------------------

    std::string sailing_id_string;

    continuouslyPromptForString(
        "Please enter the ID of the sailing [TTT-dd-hh]: ",
        g_sailing_id_regex,
        sailing_id_string
        );

    std::string departure_terminal = "";
    int departure_day = 0;
    int departure_hour = 0;

    Utilities::extractSailingID(
        sailing_id_string,
        departure_terminal,
        departure_day,
        departure_hour
        );


    // Check Sailing ID exists:
    // ----------------------------------------------------------------------------

    Sailing sailing;

    m_database->getSailingByID(
        departure_terminal,
        departure_day,
        departure_hour,
        sailing,
        g_is_successful,
        g_outcome_message
        );

    if(!g_is_successful)
    {
        std::cout << "\n" << g_outcome_message << "\n\n";

        m_state_manager->selectNextState(States::ReservationManagementState);

        return; // Abort the operation.
    }


    // Get Vehicle license plate:
    // ----------------------------------------------------------------------------

    std::string license_plate = "";

    continuouslyPromptForString(
        "Please enter the licence plate of the vehicle: ",
        g_license_plate_regex,
        license_plate
        );


    // Check Vehicle exits:
    // ----------------------------------------------------------------------------

    Vehicle vehicle;

    vehicle.license_plate = license_plate;

    m_database->getVehicleByID(
        license_plate,
        vehicle,
        g_is_successful,
        g_outcome_message
        );

    // If vehicle is not registered, then ask for more information and create it:
    if(!g_is_successful)
    {
        continuouslyPromptForString(
            "Please enter the phone number of the owner: ",
            g_phone_number_regex,
            vehicle.phone_number
            );

        // Vehicle length [0–99.9]:
        continuouslyPromptForReal(
            "Please enter the length of the vehicle [0-99.9]: ",
            g_vehicle_min_length,
            g_vehicle_max_length,
            vehicle.length
            );

        // Vehicle height [0–9.9]
        continuouslyPromptForReal(
            "Please enter the height of the vehicle [0-9.9]: ",
            g_vehicle_min_height,
            g_vehicle_max_height,
            vehicle.height
            );

        int new_vehicle_id = -1;

        m_database->addVehicle(
            vehicle,
            new_vehicle_id,
            g_is_successful,
            g_outcome_message
            );

        if(!g_is_successful)
        {
            std::cout << g_outcome_message << "\n\n";

            m_state_manager->selectNextState(States::ReservationManagementState);

            return; // Abort the operation.
        }

        vehicle.vehicle_id = new_vehicle_id;
    }
    std::cout << "\n";


    // Ask for confirmation and create reseravtion:
    // ----------------------------------------------------------------------------

    char user_choice = '\0';

    continuouslyPromptForCharacter(
        "Are you sure you want to create this new reservation [y/n]? ",
        g_allowed_yes_no_responses,
        user_choice
        );
    std::cout << "\n";

    switch(user_choice)
    {
    // Attempt reservation creation:
    case 'y':
    case 'Y':
        State::m_database->addReservation(
            sailing,
            vehicle,
            g_is_successful,
            g_outcome_message
            );

        std::cout << g_outcome_message << "\n";
        break;
    case 'n':
    case 'N':
        std::cout << "Reservation creation operation aborted!" << "\n";
        break;
    }
    std::cout << "\n";
}

// ****************************************************************************
void ReservationManagementState::deleteReservation()
{
    // Get Sailing ID:
    // ----------------------------------------------------------------------------

    std::string sailing_id_string;

    continuouslyPromptForString(
        "Please enter the ID of the sailing [TTT-dd-hh]: ",
        g_sailing_id_regex,
        sailing_id_string
        );

    std::string departure_terminal = "";
    int departure_day = 0;
    int departure_hour = 0;

    Utilities::extractSailingID(
        sailing_id_string,
        departure_terminal,
        departure_day,
        departure_hour
        );


    // Check Sailing ID exists:
    // ----------------------------------------------------------------------------

    Sailing sailing;

    m_database->getSailingByID(
        departure_terminal,
        departure_day,
        departure_hour,
        sailing,
        g_is_successful,
        g_outcome_message
        );

    if(!g_is_successful)
    {
        std::cout << "\n" << g_outcome_message << "\n\n";

        m_state_manager->selectNextState(States::ReservationManagementState);

        return; // Abort the operation.
    }


    // Get Vehicle license plate:
    // ----------------------------------------------------------------------------

    std::string license_plate = "";

    continuouslyPromptForString(
        "Please enter the licence plate of the vehicle: ",
        g_license_plate_regex,
        license_plate
        );
    std::cout << "\n";


    // Check Vehicle exits:
    // ----------------------------------------------------------------------------

    Vehicle vehicle;

    m_database->getVehicleByID(
        license_plate,
        vehicle,
        g_is_successful,
        g_outcome_message
        );

    // If vehicle is not registered, then abort:
    if(!g_is_successful)
    {
        std::cout << g_outcome_message << "\n\n";

        m_state_manager->selectNextState(States::ReservationManagementState);

        return; // Abort the operation.
    }


    //TODO must check if vehicle is already boarded before cancelling reservation
    // ----------------------------------------------------------------------------





    // Ask for confirmation and create reseravtion:
    // ----------------------------------------------------------------------------

    char user_choice = '\0';

    continuouslyPromptForCharacter(
        "Are you sure you want to delete this reservation [y/n]? ",
        g_allowed_yes_no_responses,
        user_choice
        );
    std::cout << "\n";

    switch(user_choice)
    {
    // Attempt reservation deletion:
    case 'y':
    case 'Y':
        State::m_database->removeReservation(
            sailing,
            vehicle,
            g_is_successful,
            g_outcome_message
            );
        std::cout << g_outcome_message << "\n\n";

        break;
    case 'n':
    case 'N':
        std::cout << "Reservation deletion operation aborted!" << "\n\n";
        break;
    }
}

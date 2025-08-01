// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*
 * [MODULE]
 *
 * Boarding State
 *
 *
 * [FILE NAME]
 *
 * boarding_state.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 – 2025/07/23 Original by Ethan
 * 
 * Rev 2 - 2025/07/25
 *       - Ethan
 *       - Made the correct outcome message be displayed when creating a 
 *         new reservation during boarding for a vehicle that didnt already have
 *         a reservation
 * 
 * Rev 3 - 2025/??/??
 *       - Ethan
 *       - replaced all hard coded regex patterns with references to global
 *       - fixed terminal output text spacing
 *       - made the correct outcome message be displayed when trying to board
 *         a vehicle that was already boarded
 * 
 *
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  - Uses static module‐scope `Vehicle s_vehicle` and `Sailing s_sailing` to persist current context across calls.
 *  - Uses static `char s_user_choice` to capture yes/no decisions.
 *
 * Memory / speed / complexity trade‑offs:
 *  - Static variables avoid repeated allocation but limit reuse and thread‑safety.
 *  - Looped database calls in `startBoarding()` ensure correctness at cost of multiple round‑trips.
 *
 * Input abstraction:
 *  - Regex‐validated prompts for sailing IDs and license plates.
 *  - Uses `continuouslyPromptForString` for repeated valid input, and `promptForCharacter` for single‐shot choices.
 *
 * Error handling & feedback:
 *  - Checks `g_is_successful` after each DB call; on failure, prints `g_outcome_message` and retries or aborts.
 *  - On missing vehicle record, collects owner info then invokes `addVehicle()`.
 *
 * Debugging & logging:
 *  - Wrapped in `#ifdef DEBUG_MODE` to log missing‐vehicle cases.
 *
 * State management:
 *  - Inherits from `State`; `onEnter()` resets static context, `onProcess()` loads sailing then calls `startBoarding()`.
 *
 * Future enhancements:
 *  - Replace static module variables with instance members for better encapsulation.
 *  - Refactor shared reservation/boarding logic to reuse code with ReservationManagementState.
 *  - Add an explicit exit option in `onProcess()` to return to main menu.
 */

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include <vector>
#include <iostream>
#include "state.hpp"
#include "boarding_state.hpp"
#include "state_manager.hpp"
#include "input.hpp"
#include "containers.hpp"
#include "database.hpp"
#include "global.hpp"
#include "utilities.hpp"

// static container for storing Vehicle info when creating a reservation
static Vehicle s_vehicle;

// static container for storing Sailing info
static Sailing s_sailing;

// static container for storing the users single character responses
static char s_user_choice;

// ----------------------------------------------------------------------------
BoardingState::BoardingState()
{
    
}

// ----------------------------------------------------------------------------
BoardingState::~BoardingState()
{
    
}

// ----------------------------------------------------------------------------
void BoardingState::onEnter()
{
    s_vehicle = Vehicle();
    s_sailing = Sailing();
}

// ----------------------------------------------------------------------------
void BoardingState::onProcess()
{
    std::string sailing_id;
    continuouslyPromptForString(
        "Please enter the ID of the sailing [TTT-dd-hh]: ",
        g_sailing_id_regex,
        sailing_id
    );

    std::string terminal_id;
    int departure_day;
    int departure_hour;
    Utilities::extractSailingID(sailing_id, terminal_id, departure_day, departure_hour);

    m_database->getSailingByID(terminal_id, departure_day, departure_hour, s_sailing, g_is_successful, g_outcome_message);

    if (!g_is_successful) 
    {
        std::cout << "\n" << g_outcome_message << "\n\n";
    }
    else 
    {
        startBoarding();
    }
    m_state_manager->selectNextState(States::MainMenuState);
}

// ----------------------------------------------------------------------------
void BoardingState::onExit()
{
    
}

// ----------------------------------------------------------------------------
void BoardingState::startBoarding()
{   
    // keep asking for vehicles to board until user wants to exit
    while (true)
    {
        //get license plate
        std::string license_plate;
        continuouslyPromptForString(
            "Please enter the license plate of the vehicle: ",
            g_license_plate_regex,
            license_plate
        );

        // try to get vehicle info
        m_database->getVehicleByID(license_plate, s_vehicle, g_is_successful, g_outcome_message);

        // didnt find the vehicle, create a new one
        if (!g_is_successful) 
        {
#ifdef DEBUG_MODE
            std::cout << "[DEBUG] didn't find vehicle record. Asking for information to create one." << std::endl;
#endif

            s_vehicle.license_plate = license_plate;
            continuouslyPromptForString(
                "Please enter the phone number of the owner: ",
                g_phone_number_regex,
                s_vehicle.phone_number
            );
            continuouslyPromptForReal(
                "Please enter the length of the vehicle [0-99.9]: ",
                g_vehicle_min_length,
                g_vehicle_max_length,
                s_vehicle.length
            );
            continuouslyPromptForReal(
                "Please enter the height of the vehicle [0-9.9]: ",
                g_vehicle_min_height,
                g_vehicle_max_height,
                s_vehicle.height
            );

            //write vehicle to database so we can use it in a reservation
            int vehicle_id;
            m_database->addVehicle(s_vehicle, vehicle_id, g_is_successful, g_outcome_message);
            s_vehicle.vehicle_id = vehicle_id; //since we didnt get s_vehicle from the database, we must fill in this
                                               //value before using s_vehicle to create a reservation or board

            if (!g_is_successful) //something went wrong, ask for vehicle info again
            {
                std::cout << g_outcome_message << "\n\n";
                continue;
            }
        }
        std::cout << "\n";

        //*******************************************************************
        // Try to board

        bool is_boarded = false;
        m_database->isBoarded(s_sailing, s_vehicle, is_boarded, g_is_successful, g_outcome_message);

        if (g_is_successful && is_boarded) //is already boarded
        {
            std::cout << g_outcome_message << "\n\n";
        }
        else
        {
            std::string reservation_outcome_message = "";
            bool reservation_is_successful = false;
            std::string boarding_outcome_message = "";
            bool boarding_is_successful = false;

            //try to board
            m_database->completeBoarding(s_sailing, s_vehicle, boarding_is_successful, boarding_outcome_message);
            if (boarding_is_successful)
            {
                std::cout << boarding_outcome_message << "\n\n";
            }
            else
            {
                //if boarding failed, it could be because there was no reservation
                //try to create a reservation for this vehicle and sailing in case it didnt exist.
                m_database->addReservation(s_sailing, s_vehicle, reservation_is_successful, reservation_outcome_message);

                std::cout << reservation_outcome_message << "\n\n";

                if (reservation_is_successful)
                {
                    //if a reservation was made now we can complete the boarding
                    m_database->completeBoarding(s_sailing, s_vehicle, boarding_is_successful, boarding_outcome_message);
                    std::cout << boarding_outcome_message << "\n\n";
                }
            }
        }

        //*********************************************************************
        // Ask user if they want to break

        promptForCharacter(
            "Do you wish to complete another boarding? [y/n]? ",
            g_allowed_yes_no_responses,
            s_user_choice,
            g_is_successful,
            g_outcome_message
        );
        std::cout << "\n";

        bool user_wants_to_break = false;

        switch (s_user_choice)
        {
        case 'n':
        case 'N':
            user_wants_to_break = true;
        }

        if (user_wants_to_break)
        {
            break;
        }
    }
}

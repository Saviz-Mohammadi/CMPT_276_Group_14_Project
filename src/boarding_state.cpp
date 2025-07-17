
#include <vector>
#include <iostream>
#include "state.hpp"
#include <regex>
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
        std::regex(R"([A-Z]{3}-\d\d-\d\d)"),
        sailing_id
    );

    std::string terminal_id;
    int departure_day;
    int departure_hour;
    Utilities::extractSailingID(sailing_id, terminal_id, departure_day, departure_hour);

    m_database->getSailingByID(terminal_id, departure_day, departure_hour, s_sailing, g_is_successful, g_outcome_message);

    if (!g_is_successful) 
    {
        std::cout << "\n\n" << g_outcome_message << "\n\n";
        return;
    }

    startBoarding();
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
            std::regex(R"([A-Z\d -]{2,10})"), //regex pattern : 2-10 capital letters, digits, spaces, hyphens
            license_plate
        );

        // try to get vehicle info
        m_database->getVehicleByID(license_plate, s_vehicle, g_is_successful, g_outcome_message);

        // didnt find the vehicle, create a new one
        if (!g_is_successful) 
        {
            s_vehicle.license_plate = license_plate;
            continuouslyPromptForString(
                "Please enter the phone number of the owner: ",
                std::regex(R"(\d{8,14})"), //regex pattern : 8-14 digits
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
        std::cout << "\n\n";

        //try to create a reservation for this vehicle and sailing in case it didnt exist.
        //if one already exists then this will fail
        m_database->addReservation(s_sailing, s_vehicle, g_is_successful, g_outcome_message);

        //complete the boarding for this vehicle
        m_database->completeBoarding(s_sailing, s_vehicle, g_is_successful, g_outcome_message);

        if (g_is_successful) 
        {
            std::cout << "Boarding completed!" << "\n\n";
        }
        else
        {
            std::cout << g_outcome_message << "\n\n";
        }

        promptForCharacter(
            "Do you wish to complete another boarding? [y/n]? ",
            g_allowed_yes_no_responses,
            s_user_choice,
            g_is_successful,
            g_outcome_message
        );

        switch (s_user_choice)
        {
        case 'y':
        case 'Y':
            continue;
        }
        break;
    }
}
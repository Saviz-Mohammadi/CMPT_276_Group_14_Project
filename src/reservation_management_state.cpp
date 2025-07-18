// 1 amount 0, 
//2 check sailing exist and abort to state
//3 if vehcile doesnot exist call addvehicle to create it without any id
//4 calulation for lanes (saviz) ASK Group about this



#include <vector>
#include <iostream>
#include "state.hpp"
#include <regex>
#include "reservation_management_state.hpp"
#include "state_manager.hpp"
#include "input.hpp"
#include "containers.hpp"
#include "database.hpp"
#include "global.hpp"

// static container for storing Reservation info when creating a reservation
static Reservation s_reservation;

// static container for storing Vehicle info when creating a reservation
static Vehicle s_vehicle;

// static container for storing Sailing info
static Sailing s_sailing;

// static container for storing the users single character responses
static char s_user_choice;

// ----------------------------------------------------------------------------
ReservationManagementState::ReservationManagementState()
{
    
}

// ----------------------------------------------------------------------------
ReservationManagementState::~ReservationManagementState()
{
    
}

// ----------------------------------------------------------------------------
void ReservationManagementState::onEnter()
{
     s_reservation = Reservation();
     s_vehicle = Vehicle();
     s_sailing = Sailing();
    std::cout <<
        "RESERVATION MANAGEMENT MENU\n"
        "1) Make a new reservation\n"
        "2) Cancel a reservation\n"
        "0) Exit to main menu\n"
        "\n";
}

// ----------------------------------------------------------------------------
void ReservationManagementState::onProcess()
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

        if (!g_is_successful)
        {
            std::cout << g_outcome_message << "\n\n";
        }
    } while (!g_is_successful);

    // Switch on selection and start the appropriate action:
    switch (s_user_choice)
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

// ----------------------------------------------------------------------------
void ReservationManagementState::onExit()
{
    
}

// ----------------------------------------------------------------------------
void ReservationManagementState::createReservation()
{   
    std::string sailing_data;

      //Sailing ID (assuming format: 3 letters-2 digits-2 digits like "AHS-22-10")
    std::regex sailing_pattern("[A-Z]{3}-\\d{2}-\\d{2}");
    continuouslyPromptForString("Please enter the ID of the sailing [TTT-dd-hh]: ", sailing_pattern,sailing_data); 

    // Parse components
    std::string terminal = sailing_data.substr(0, 3);
    int day = std::stoi(sailing_data.substr(4, 2));
    int hour = std::stoi(sailing_data.substr(7, 2));

    //savis is xhanging this

    m_database->getSailingByID(terminal, day, hour, s_sailing, g_is_successful, g_outcome_message);

    if (!g_is_successful) {
        std::cout << g_outcome_message << "\n\n";
        m_state_manager->selectNextState(States::ReservationManagementState);
    }    

    //License plate (pattern A76-2H4)
    std::regex plate_pattern("[A-Z0-9]{3}-[A-Z0-9]{3}");
    continuouslyPromptForString("Please enter the licence plate of the vehicle: ", plate_pattern,s_vehicle.license_plate);

    m_database->getVehicleByID(s_vehicle.license_plate,s_vehicle,g_is_successful,g_outcome_message);

    if (!g_is_successful) {
         //Phone number (12-digit only digits)
        std::regex phone_pattern("\\d{12}");
        continuouslyPromptForString("Please enter the phone number of the owner: ", phone_pattern, s_vehicle.phone_number);

        //Vehicle length [0–99.9]
        continuouslyPromptForReal("Please enter the length of the vehicle [0-99.9]: ", 0, 99, s_vehicle.length);

        //Vehicle height [0–9.9]
        continuouslyPromptForReal("Please enter the height of the vehicle [0-9.9]: ", 0.0, 9.9, s_vehicle.height);

        m_database->addVehicle(s_vehicle,s_vehicle.vehicle_id,g_is_successful,g_outcome_message);
        if (!g_is_successful) {
        std::cout << g_outcome_message << "\n\n";
        m_state_manager->selectNextState(States::ReservationManagementState);
        } 
    }

    char confirm; //to variable to take userinput to confirm the creation of reservation.

    //Confirmation
    continuouslyPromptForCharacter("Are you sure you want to create this new reservation [y/n]? ", {'y', 'n'}, confirm);

    // Final message
    if (confirm == 'y') {
        m_database->addReservation(s_sailing,s_vehicle,g_is_successful,g_outcome_message);
        if (!g_is_successful) {
        std::cout << g_outcome_message << "\n\n";
        m_state_manager->selectNextState(States::ReservationManagementState);
        } 
        std::cout << "\nNew reservation successfully created!\n";
        
    } else {
        std::cout << "\nReservation Abort.\n";
    }
    m_state_manager->selectNextState(States::MainMenuState);
}

// ----------------------------------------------------------------------------
void ReservationManagementState::deleteReservation()
{
    std::string sailing_data;

      //Sailing ID (assuming format: 3 letters-2 digits-2 digits like "AHS-22-10")
    std::regex sailing_pattern("[A-Z]{3}-\\d{2}-\\d{2}");
    continuouslyPromptForString("Please enter the ID of the sailing [TTT-dd-hh]: ", sailing_pattern,sailing_data); 

    // Parse components
    std::string terminal = sailing_data.substr(0, 3);
    int day = std::stoi(sailing_data.substr(4, 2));
    int hour = std::stoi(sailing_data.substr(7, 2));

    m_database->getSailingByID(terminal, day, hour, s_sailing, g_is_successful, g_outcome_message);

    if (!g_is_successful) {
        std::cout << g_outcome_message << "\n\n";
        m_state_manager->selectNextState(States::ReservationManagementState);
    }    

    //License plate (pattern A76-2H4)
    std::regex plate_pattern("[A-Z0-9]{3}-[A-Z0-9]{3}");
    continuouslyPromptForString("Please enter the licence plate of the vehicle: ", plate_pattern,s_vehicle.license_plate);

    m_database->getVehicleByID(s_vehicle.license_plate,s_vehicle,g_is_successful,g_outcome_message);
    if (!g_is_successful) {
        std::cout << g_outcome_message << "\n\n";
        m_state_manager->selectNextState(States::ReservationManagementState);
    }

    char confirm; //to variable to take userinput to confirm the creation of reservation.

    //Confirmation
    continuouslyPromptForCharacter("Are you sure you want to cancel this reservation [y/n]? ", {'y', 'n'}, confirm);

    // Final message
    if (confirm == 'y') {
        m_database->removeReservation(s_sailing,s_vehicle,g_is_successful,g_outcome_message);
        if (!g_is_successful) {
        std::cout << g_outcome_message << "\n\n";
        m_state_manager->selectNextState(States::ReservationManagementState);
        } 
        std::cout << "\nReservation successfully cancelled!\n";
        
    } else {
        std::cout << "\nReservation cancelling operation aborted!\n";
    }
    m_state_manager->selectNextState(States::MainMenuState);
    
    
}
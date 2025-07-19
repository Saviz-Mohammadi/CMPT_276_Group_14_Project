#include <iostream>
#include "input.hpp"
#include "main_menu_state.hpp"
#include "state_manager.hpp"
#include "database.hpp"
#include "global.hpp"

// ----------------------------------------------------------------------------
MainMenuState::MainMenuState()
{

}

// ----------------------------------------------------------------------------
MainMenuState::~MainMenuState()
{

}

// ----------------------------------------------------------------------------
void MainMenuState::onEnter()
{
    std::cout <<
        "MAIN MENU\n"
        "1) Go to vessel management menu\n"
        "2) Go to sailing management menu\n"
        "3) Go to reservation management menu\n"
        "4) Complete boarding\n"
        "0) Exit program\n"
        "\n";
}

// ----------------------------------------------------------------------------
void MainMenuState::onProcess()
{
    char user_choice;
    continuouslyPromptForCharacter(
        "Please enter yout selection [0-4]: ", 
        std::vector<char>{'0','1','2','3','4'},
        user_choice
        );
    std::cout << "\n";

    switch (user_choice) {    
    case '1':
        m_state_manager->selectNextState(States::VesselManagementState);
        break;
    case '2':
        m_state_manager->selectNextState(States::SailingManagementState);
        break;
    case '3':
        m_state_manager->selectNextState(States::ReservationManagementState);
        break;
    case '4':
        m_state_manager->selectNextState(States::BoardingState);
        break;
    case '0':
    default:
        //TODO: any shutdown procedure to put here? or in onExit()?
        m_state_manager->selectNextState(States::ExitState);
        break;
    }
}

// ----------------------------------------------------------------------------
void MainMenuState::onExit()
{

}

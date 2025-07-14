
#include <vector>
#include <iostream>
#include <regex>
#include <ctime>
#include <iomanip>
#include "state.hpp"
#include "sailing_management_state.hpp"
#include "state_manager.hpp"
#include "input.hpp"
#include "containers.hpp"
#include "database.hpp"
#include "global.hpp"

static Sailing s_sailing;
static char s_user_choice;
static bool s_is_successful;
static std::string s_outcome_message;

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
    s_sailing = Sailing();
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
    // Get user choice:
    do
    {
        promptForCharacter(
            "Please enter your selection [0-4]: ",
            std::vector<char>{'0', '1', '2', '3', '4'},
            s_user_choice,
            s_is_successful,
            s_outcome_message
        );

        if (!s_is_successful)
        {
            std::cout << s_outcome_message << "\n\n";
        }
    } while (!s_is_successful);

    // Switch on selection and start the appropriate action:
    switch (s_user_choice)
    {
    case '1':
        createSailing();
        m_state_manager->selectNextState(States::SailingManagementState);
        break;
    case '2':
        deleteSailing();
        m_state_manager->selectNextState(States::SailingManagementState);
        break;
    case '3':
        listSailingReports();
        m_state_manager->selectNextState(States::SailingManagementState);
        break;
    case '4':
        listSailingReport();
        m_state_manager->selectNextState(States::SailingManagementState);
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
void SailingManagementState::createSailing()
{

}

// ----------------------------------------------------------------------------
void SailingManagementState::deleteSailing()
{

}

// ----------------------------------------------------------------------------
void SailingManagementState::listSailingReports()
{

}

// ----------------------------------------------------------------------------
void SailingManagementState::listSailingReport()
{

}
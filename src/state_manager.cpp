#include "state_manager.hpp"

StateManager::StateManager()
{
    m_state = nullptr;
}

StateManager::~StateManager()
{
}

void StateManager::init(
    Database* database
    )
{
    // Selecting starting state:
    m_state = &m_main_menu_state;

    m_main_menu_state.init(
        this,
        database
        );

    m_vessel_management_state.init(
        this,
        database
        );

    m_sailing_management_state.init(
        this,
        database
        );

    m_reservation_management_state.init(
        this,
        database
        );

    m_boarding_state.init(
        this,
        database
        );
}

void StateManager::run()
{
    while(m_state != nullptr)
    {
        State *current_state = m_state; //onProcess() changes m_state so make a copy of the pointer
        current_state->onEnter();
        current_state->onProcess();
        current_state->onExit();
	}
}

void StateManager::selectNextState(
    States next_state
    )
{
    switch(next_state)
    {
    case States::MainMenuState:
		m_state = &m_main_menu_state;
		break;
    case States::VesselManagementState:
		m_state = &m_vessel_management_state;
		break;
    case States::SailingManagementState:
		m_state = &m_sailing_management_state;
		break;
    case States::ReservationManagementState:
		m_state = &m_reservation_management_state;
		break;
    case States::BoardingState:
        m_state = &m_boarding_state;
		break;
    case States::ExitState:
		m_state = nullptr;
		break;
	}
}

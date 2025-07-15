
#include "state_manager.hpp"

StateManager::StateManager()
{
	
}

StateManager::~StateManager()
{

}

StateManager::init(Database* database)
{
	m_state = &m_main_menu_state;;

	m_main_menu_state.init(&this, database);
	m_vessel_management_state.init(&this, database);
	m_sailing_management_state.init(&this, database);
	m_reservation_management_state.init(&this, database);
	m_boarding_state.init(&this, database);
}

StateManager::run()
{
	while (m_state != nullptr) {		
		State* current_state = m_state; // onProcess changes m_state so make a copy
		current_state.onEnter();
		current_state.onProcess();
		current_state.onExit();
	}
}

StateManager::selectNextState(States next_state)
{
	switch (next_state) {
	case (States::MainMenuState):
		m_state = &m_main_menu_state;
		break;
	case (States::VesselManagementState):
		m_state = &m_vessel_management_state;
		break;
	case (States::SailingManagementState):
		m_state = &m_sailing_management_state;
		break;
	case (States::ReservationsManagementState):
		m_state = &m_reservation_management_state;
		break;
	case (States::BoardingState):
		m_state = &m_bearding_state;
		break;
	case (States::ExitState):
		m_state = nullptr;
		break;
	}
}
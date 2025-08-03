// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
 * [FILE NAME]
 *
 * state_manager.cpp
 *
 * [REVISION HISTORY]
 *
 * Rev 1 – 2025/07/23 Original by Saviz Mohammadi
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  - Maintains an internal pointer `m_state` to the active `State` instance.
 *  - Holds concrete state instances as members: main menu, vessel management, sailing management, reservation management, boarding.
 *
 * Memory / speed / complexity trade‑offs:
 *  - States are stored as members (no heap allocations) for minimal overhead.
 *  - Uses raw pointers for state transitions: fast, but no ownership safety.
 *
 * Initialization & lifecycle:
 *  - Default constructor zeroes `m_state` pointer.
 *  - `init(database)` binds each state instance to this manager and the shared database.
 *  - `run()` dispatches `onEnter`/`onProcess`/`onExit` in a loop until `m_state` becomes `nullptr`.
 *
 * State transition logic:
 *  - `selectNextState` switches `m_state` based on the `States` enum.
 *  - Exit state sets `m_state` to `nullptr`, terminating the run loop.
 *
 * Error handling & assumptions:
 *  - Assumes `init` is always called before `run()`.
 *  - No null‐checks on `m_state` inside `run()` loop apart from the loop condition.
 */

#include "state_manager.hpp"

// ----------------------------------------------------------------------------
StateManager::StateManager()
{
    m_state = nullptr;
}

// ----------------------------------------------------------------------------
StateManager::~StateManager()
{
}

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
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

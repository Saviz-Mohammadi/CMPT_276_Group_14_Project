// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Finite State Machine Module
 *
 *
 * [FILE NAME]
 *
 * state_manager.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a centralized location for orchestrating all states and managing transitions between them.
*/

// ============================================================================
// ============================================================================

#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include "state.hpp"
#include "main_menu_state.hpp"
#include "vessel_management_state.hpp"
#include "sailing_management_state.hpp"
#include "reservation_management_state.hpp"
#include "boarding_state.hpp"

class Database;

// An enum used to refer to each State when attempting to make a transition.
enum class States
{
    MainMenuState,
    VesselManagementState,
    SailingManagementState,
    ReservationManagementState,
    BoardingState,
    ExitState
};

class StateManager
{
public:
    // ----------------------------------------------------------------------------
    explicit StateManager();

    /*
    *   [Description]
    *   Constructor for the StateManager class. used to instantiate a physical object in memory.
    *   Won't do any heavy work.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    ~StateManager() override;

    /*
    *   [Description]
    *   Destructor for the StateManager class. Ensures proper cleanup of resources and polymorphic destruction.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------
    
public:
    // ----------------------------------------------------------------------------
    void init(
        Database* database // [IN] | A pointer to the database instance that each underlying State will require.
        );

    /*
    *   [Description]
    *   This function attempts to initialize the required member fields with data.
    *   It will create each underlying State and make sure they get pointers/references to the data that they require.
    *   It is important to call this method before invoking anything else in the StateManager.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void run();

    /*
    *   [Description]
    *   This function triggers the main Finite State Machine loop by calling the 'onEnter()', 'onProcess()', and 'onExit()' methods of each underlying state.
    *   The loop begins with the initial state, which is MainMenuState.
    *   It is important to call 'init()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void selectNextState(
        States next_state // [IN] | An enum indicating the next State to transition to.
        );

    /*
    *   [Description]
    *   This function allows each underlying state to specify the next state that the StateManager should transition to.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------

private:
    // A polymorphic instance to reference each state.
    State* m_state;

    // Concrete instances of each state.
    MainMenuState m_main_menu_state;
    VesselManagementState m_vessel_management_state;
    SailingManagementState m_sailing_management_state;
    ReservationManagementState m_reservation_management_state;
    BoardingState m_boarding_state;
};

#endif // STATEMANAGER_HPP


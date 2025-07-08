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
 * state.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a base for all other states to inherit from and adhere to.
*/

// ============================================================================
// ============================================================================

#ifndef STATE_HPP
#define STATE_HPP

class StateManager;
class Database;

class State
{
public:
    // ----------------------------------------------------------------------------
    explicit State();

    /*
    *   [Description]
    *   Constructor for the State base class. Does not perform any heavy work.
    *   It simply sets up the v-table and prepares the object for use by a derived state.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    virtual ~State();

    /*
    *   [Description]
    *   Virtual destructor for the State base class. Ensures that destruction is polymorphic.
    *   This means that all subclasses’ cleanup code is invoked correctly.
    *   Derived destructors must handle their own teardown logic.
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
        StateManager* state_manager,
        Database* database
        );

    /*
    *   [Description]
    *   This function attempts to initialize the required member fields with data.
    *   It is important to call this method before invoking anything else in the State.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------

public:
    // ----------------------------------------------------------------------------
    virtual void onEnter() = 0;

    /*
    *   [Description]
    *   This pure virtual function defines the behavior that should occur when entering a state.
    *   Subclasses should use this method as a setup phase for initializing variables, displaying initial output, and related tasks.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    virtual void onProcess() = 0;

    /*
    *   [Description]
    *   This pure virtual function defines the primary behavior that should occur while the system is in a given state.
    *   Subclasses should implement this method to handle the main loop of the state, gather input, and invoke relevant logic.
    *   It should also determine the next state to transition to upon exiting, based on applicable conditions.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    virtual void onExit() = 0;

    /*
    *   [Description]
    *   This pure virtual function defines the behavior that should occur when exiting a state.
    *   Subclasses should use this method as a cleanup phase for releasing resources, resetting variables, and performing other teardown tasks.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------
    
protected:
    // A pointer to the 'StateManager', allowing access to methods related to state transitions and management.
    StateManager* m_state_manager;

    // A pointer to the 'Database', providing access to underlying database functionality.
    Database* m_database;
};

#endif // STATE_HPP

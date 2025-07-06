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
    explicit State();
    virtual ~State();
    
public:
    void init(StateManager* state_manager, Database* database);

public:
    virtual void onEnter() = 0;
    virtual void onProcess() = 0;
    virtual void onExit() = 0;
    
protected:
    StateManager* m_state_manager;
    Database* m_database;
};

#endif // STATE_HPP

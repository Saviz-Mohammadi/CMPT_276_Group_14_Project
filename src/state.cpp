// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * State
 *
 *
 * [FILE NAME]
 *
 * state.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/23 Original by Saviz Mohammadi
 *
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  - Holds pointers to `StateManager` and `Database` to coordinate state transitions and persistence.
 *
 * Memory / speed / complexity trade‑offs:
 *  - Uses raw pointers for minimal overhead; no ownership safety or automatic lifetime management.
 *
 * Initialization & lifecycle:
 *  - Default constructor initializes pointers to `nullptr` to avoid dangling references.
 *  - `init(...)` method binds external dependencies after object construction.
 *
 * Error handling & assumptions:
 *  - No null‑pointer checks: assumes `init` is always called before any use of the pointers.
 *
 * Future enhancements:
 *  - Consider replacing raw pointers with smart pointers or references for stronger ownership semantics.
 *  - Add assertions, logging, or error checks to catch uninitialized usage.
 *
 */

// ============================================================================
// ============================================================================


#include "state.hpp"

State::State()
{ 
    this->m_database = nullptr;
    this->m_state_manager = nullptr;
}

State::~State()
{
}

void State::init(StateManager* state_manager, Database* database)
{
    this->m_state_manager = state_manager;
    this->m_database = database;
}

#include "state.hpp"

State::State()
{ 
    this->m_database = nullptr;
    this->m_state_manager = nullptr;
}

void State::init(StateManager* state_manager, Database* database)
{
    this->m_state_manager = state_manager;
    this->m_database = database;
}
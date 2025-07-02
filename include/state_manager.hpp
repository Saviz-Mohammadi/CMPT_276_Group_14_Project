#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "state.hpp"
#include "main_menu_state.hpp"
#include "vessel_management_state.hpp"

enum class States
{
    MainMenuState,
    VesselManagementState,
    ExitState
};

class StateManager
{
public:
    explicit StateManager();
    ~StateManager();
    
public:
    void run();
    void selectNextState(States next_state);

private:
    State* m_state;

    // Concrete instances:
    MainMenuState m_main_menu_state;
    VesselManagementState m_vessel_management_state;
};

#endif // STATEMANAGER_H


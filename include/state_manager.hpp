#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include "state.hpp"
#include "main_menu_state.hpp"
#include "vessel_management_state.hpp"
#include "sailing_management_state.hpp"
#include "reservation_management_state.hpp"
#include "boarding_state.hpp"

class Database;

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
    explicit StateManager();
    ~StateManager();
    
public:
    void init(Database* database);
    void run();
    void selectNextState(States next_state);

private:
    State* m_state;

    // Concrete instances:
    MainMenuState m_main_menu_state;
    VesselManagementState m_vessel_management_state;
    SailingManagementState m_sailing_management_state;
    ReservationManagementState m_reservation_management_state;
    BoardingState m_boarding_state;
};

#endif // STATEMANAGER_HPP


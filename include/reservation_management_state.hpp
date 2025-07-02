#ifndef RESERVATION_MANAGEMENT_STATE_HPP
#define RESERVATION_MANAGEMENT_STATE_HPP

#include "state.hpp"

class ReservationManagementState : public State
{
public:
    explicit ReservationManagementState();
    ~ReservationManagementState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;
};

#endif // RESERVATION_MANAGEMENT_STATE_HPP

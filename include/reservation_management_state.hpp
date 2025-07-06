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
 * reservation_management_state.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a state/mode where we can complete operations regarding reservations.
*/

// ============================================================================
// ============================================================================

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

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
 * bording.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a state/mode where we can complete operations regarding boarding.
*/

// ============================================================================
// ============================================================================

#ifndef BOARDING_STATE_HPP
#define BOARDING_STATE_HPP

#include "state.hpp"

class BoardingState : public State
{
public:
    explicit BoardingState();
    ~BoardingState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;
};

#endif // BOARDING_STATE_HPP

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
 * vessel_management_state.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a state/mode where we can complete operations regarding vessels.
*/

// ============================================================================
// ============================================================================

#ifndef VESSEL_MANAGEMENT_STATE_HPP
#define VESSEL_MANAGEMENT_STATE_HPP

#include "state.hpp"

class VesselManagementState : public State
{
public:
    explicit VesselManagementState();
    ~VesselManagementState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;
};

#endif // VESSEL_MANAGEMENT_STATE_HPP

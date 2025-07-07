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
 * sailing_management_state.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a state/mode where we can complete operations regarding sailings.
*/

// ============================================================================
// ============================================================================

#ifndef SAILING_MANAGEMENT_STATE_HPP
#define SAILING_MANAGEMENT_STATE_HPP

#include "state.hpp"

class SailingManagementState : public State
{
public:
    explicit SailingManagementState();
    ~SailingManagementState() override;
    
public:
    void onEnter() override;
    void onProcess() override;
    void onExit() override;

private:
    void createSailing();
    void deleteSailing();
    void listSailingReports();
    void listSailingReport();
};

#endif // SAILING_MANAGEMENT_STATE_HPP

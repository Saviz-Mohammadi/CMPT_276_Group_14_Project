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
    // ----------------------------------------------------------------------------
    explicit ReservationManagementState();

    /*
    *   [Description]
    *   Constructor for the ReservationManagementState class. used to instantiate a physical object in memory.
    *   Won't do any heavy work.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    ~ReservationManagementState() override;

    /*
    *   [Description]
    *   Destructor for the ReservationManagementState class. Ensures proper cleanup of resources and polymorphic destruction.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------

public:
    // ----------------------------------------------------------------------------
    void onEnter() override;

    /*
    *   [Description]
    *   Called when entering the reservation management state. Use this method to display menus,
    *   initialize UI components, or reset state-specific variables.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void onProcess() override;

    /*
    *   [Description]
    *   Main processing loop for the reservation management state. Handles user input,
    *   delegates actions such as creating, deleting reservations.
    *   Also, determines state transitions.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void onExit() override;

    /*
    *   [Description]
    *   Called when exiting the reservation management state. Perform any necessary cleanup,
    *   such as clearing buffers or saving temporary data.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------

private:
    // ----------------------------------------------------------------------------
    void createReservation();

    /*
    *   [Description]
    *   Initiates the workflow for creating a reservation entry (sailing remaining length will be reduced as a result of this).
    *   Obtains and validates input with the help of input module.
    *   In addition, interacts with the databse module to create the reservation.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void deleteReservation();

    /*
    *   [Description]
    *   Initiates the workflow for deleting a reservation entry (sailing remaining length will be increased as a result of this).
    *   Obtains and validates input with the help of input module.
    *   In addition, interacts with the databse module to delete the reservation.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------
};

#endif // RESERVATION_MANAGEMENT_STATE_HPP

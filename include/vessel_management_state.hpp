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
    // ----------------------------------------------------------------------------
    explicit VesselManagementState();

    /*
    *   [Description]
    *   Constructor for the VesselManagementState class. used to instantiate a physical object in memory.
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
    ~VesselManagementState() override;
    /*
    *   [Description]
    *   Destructor for the VesselManagementState class. Ensures proper cleanup of resources and polymorphic destruction.
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
    *   Called when entering the vessel management state. Use this method to display menus,
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
    *   Main processing loop for the vessel management state. Handles user input,
    *   delegates actions such as creating or listing vessels, and determines state transitions.
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
    *   Called when exiting the vessel management state. Perform any necessary cleanup,
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
    void creatVessel();

    /*
    *   [Description]
    *   Initiates the workflow for creating a new vessel entry.
    *   Obtains and validates input with the help of the input module.
    *   In addition, interacts with the database module to persist the new vessel.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void listVessels();

    /*
    *   [Description]
    *   Initiates the workflow for listing vessels (with a count limit).
    *   Interacts with the database module to obtain the list of vessels.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------
};

#endif // VESSEL_MANAGEMENT_STATE_HPP

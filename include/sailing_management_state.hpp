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
    // ----------------------------------------------------------------------------
    explicit SailingManagementState();

    /*
    *   [Description]
    *   Constructor for the SailingManagementState class. used to instantiate a physical object in memory.
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
    ~SailingManagementState() override;

    /*
    *   [Description]
    *   Destructor for the SailingManagementState class. Ensures proper cleanup of resources and polymorphic destruction.
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
    *   Called when entering the sailing management state. Use this method to display menus,
    *   initialize UI components, or reset state-specific variables.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void onProcess() override;

    /*
    *   [Description]
    *   Main processing loop for the sailing management state. Handles user input,
    *   delegates actions such as creating, deleting sailings, and listing sailing reports.
    *   Also, determines state transitions.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void onExit() override;

    /*
    *   [Description]
    *   Called when exiting the sailing management state. Perform any necessary cleanup,
    *   such as clearing buffers or saving temporary data.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------

private:
    // ----------------------------------------------------------------------------
    void createSailing();

    /*
    *   [Description]
    *   Initiates the workflow for creating a new sailing entry.
    *   Obtains and validates input with the help of input module.
    *   In addition, interacts with the databse module to persist the sailing.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void deleteSailing();

    /*
    *   [Description]
    *   Initiates the workflow for deleting a sailing entry.
    *   Obtains and validates input with the help of input module.
    *   In addition, interacts with the databse module to delete the sailing as well as reservations associated with the sailing.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void listSailingReports();

    /*
    *   [Description]
    *   Initiates the workflow for retrieving and displaying a summary of sailing reports (with a certain count).
    *   Accomplishes this by interacting with the databse module to obtain the list of sailing reports.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    void listSailingReport();

    /*
    *   [Description]
    *   Initiates the workflow for retrieving and displaying a summary of sailing report (just one).
    *   Accomplishes this by interacting with the databse module to obtain the sailing report.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------
};

#endif // SAILING_MANAGEMENT_STATE_HPP

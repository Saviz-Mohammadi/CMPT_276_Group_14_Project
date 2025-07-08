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
    // ----------------------------------------------------------------------------
    explicit BoardingState();

    /*
    *   [Description]
    *   Constructor for the BoardingState class. used to instantiate a physical object in memory.
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
    ~BoardingState() override;

    /*
    *   [Description]
    *   Destructor for the BoardingState class. Ensures proper cleanup of resources and polymorphic destruction.
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
    *   Called when entering the boarding state. Use this method to display menus,
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
    *   Main processing loop for the boarding state. Handles user input,
    *   delegates actions such as start boarding, and determines state transitions.
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
    *   Called when exiting the boarding state. Perform any necessary cleanup,
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
    void startBoarding();

    /*
    *   [Description]
    *   Initiates the boarding process for passengers.
    *   Obtains and validates input with the help of the input module.
    *   In addition, interacts with the Database to record boardings.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------
};

#endif // BOARDING_STATE_HPP

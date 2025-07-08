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
 * main_menu_state.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a state/mode where we can complete operations regarding switching to other menus and exiting.
*/

// ============================================================================
// ============================================================================

#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include "state.hpp"

class MainMenuState : public State
{
public:
    // ----------------------------------------------------------------------------
    explicit MainMenuState();

    /*
    *   [Description]
    *   Constructor for the MainMenuState class. used to instantiate a physical object in memory.
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
    ~MainMenuState() override;

    /*
    *   [Description]
    *   Destructor for the MainMenuState class. Ensures proper cleanup of resources and polymorphic destruction.
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
    *   Called when entering the main menu state. Use this method to display menus,
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
    *   Main processing loop for the main menu state. Handles user input,
    *   and transitions to other states and menus mainly.
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
    *   Called when exiting the main menu state. Perform any necessary cleanup,
    *   such as clearing buffers or saving temporary data.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   This method is designed to handle all errors internally, ensuring that external components do not need to manage exception handling for its operations.
    */
    // ----------------------------------------------------------------------------
};

#endif // MAIN_MENU_STATE_HPP

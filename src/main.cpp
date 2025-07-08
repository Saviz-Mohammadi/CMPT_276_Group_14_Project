// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Main Module
 *
 *
 * [FILE NAME]
 *
 * main.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for initializing the application by opening the database connection,
 * set up and run the state manager to drive the program’s main loop,
 * and then close the database and clean up all resources before exit.
*/

// ============================================================================
// ============================================================================

#include <sqlite3.h>
#include "state_manager.hpp"
#include "database.hpp"

int main(int argc, char *argv[])
{
    // bool is_successful = false;
    // std::string outcome_message = "";

    //  Section: Database setup
    // ------------------------------------------------------------------------

    /*

    Database *database = new Database();

    database->openConnection("database.db", is_successful, outcome_message);

    // If the operation is not successful, then just abort:
    if(!is_successful)
    {
        std::cout << outcome_message << std::endl;

        return(0);
    }

    */

    // ------------------------------------------------------------------------



    //  Section: State setup
    // ------------------------------------------------------------------------

    /*

    StateManager state_manager;

    state_manager.init(database);
    state_manager.run();

    */

    // ------------------------------------------------------------------------



    //  Section: Cleanup
    // ------------------------------------------------------------------------

    /*

    database->cutConnection(is_successful, outcome_message);

    // If the operation is not successful, then just print message:
    if(!is_successful)
    {
        std::cout << outcome_message << std::endl;
    }

    delete database;

    */

    // ------------------------------------------------------------------------


    return(0);
}

/* CONVENTIONS

[Dividers]

-- We will use the following major divider per each file at the very top to explain the definition of the module:
// ============================================================================

-- We will use the following divider to seperate each function:
// ----------------------------------------------------------------------------

-- We will use the following divider per each major section of our code to categorize long logical parts:
// ****************************************************************************


[File conventions]
-- File names will use 'Snake Case' as the naming convention where all words are written in lowercase and separated by underscores. (this_is_snake_case)


[Variable conventions]
-- By default we will use 'Snake Case' as the naming convention where all words are written in lowercase and separated by underscores. (this_is_snake_case)
-- Static types will use 'Snake Case' with a 's_' prefix. (s_my_variable)
-- Constant types will use 'Snake Case' with a 'c_' prefix. (c_my_variable)
-- Static and Constant types combined will use 'Snake Case' with a 'sc_' prefix. (sc_my_variable)


[Class conventions]
-- Class signature names will use 'Pascal Case' as the naming convention where multiple words are joined together with no underscores or spaces, and each word starts with a capital letter. (ThisIsPascalCase)
-- Member variables in classes are prefixed with 'm_' to indicate that they are member variables. (m_my_variable)
-- Each class should list its methods and members in the following order: public first, followed by protected, and then private.


[Struct conventions]
-- Struct signature names will use 'Pascal Case' as the naming convention where multiple words are joined together with no underscores or spaces, and each word starts with a capital letter. (ThisIsPascalCase)
-- Member variables in structs are not prefixed and are typically named to directly match the data they represent. This is because structs are primarily used to group raw data and serve as temporary containers for data transfer.
-- Each class should list its methods and members in the following order: public first, followed by protected, and then private.


[Function/Method conventions]
-- Function/Method signature names will use 'Camel Case' as the naming convention where multiple words are joined together with no underscores or spaces, and each word after the first starts with an uppercase letter. (thisIsCamelCase)
-- Reference and pointer parameters, whether 'in' or 'out', will not use prefixes or suffixes like 'in_' or 'out_', as these can obscure the meaning of the parameter. Instead, their roles will be documented through comments. Additionally, 'out' parameters will generally follow 'in' parameters in the parameter list.


[Spacing conventions]
-- Tabs will be automatically be converted to and replaced with spaces of length 4. (IDE settings)
-- There will be no space before and after grouping symbols such '()', '{}', '<>'.
-- Curly brackets '{}' will be placed on the next line for classes, functions, loops, switches, and others for better readability.
-- There will be spaces before and after '=' signs.
-- There will be spaces after commas.

*/

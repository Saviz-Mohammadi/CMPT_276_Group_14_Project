#include <sqlite3.h>

int main(int argc, char *argv[])
{
    //  Section: Database setup
    //-------------------------------------------------------------------------

    /*

    Database *database = new Database();

    database->openConnection("database.db", g_is_successful, g_outcome_message);

    // If the operation is not successful, then just abort:
    if(!g_is_successful)
    {
        std::cout << g_outcome_message << std::endl;

        return(0);
    }

    */

    //-------------------------------------------------------------------------



    //  Section: State setup
    //-------------------------------------------------------------------------

    /*

    StateManager state_manager;

    state_manager.init(database);
    state_manager.run();

    */

    //-------------------------------------------------------------------------



    //  Section: Cleanup
    //-------------------------------------------------------------------------

    /*

    database->cutConnection(g_is_successful, g_outcome_message);

    // If the operation is not successful, then just print message:
    if(!g_is_successful)
    {
        std::cout << g_outcome_message << std::endl;
    }

    delete database;

    */

    //-------------------------------------------------------------------------


    return(0);
}

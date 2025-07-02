#include <sqlite3.h>

int main(int argc, char *argv[])
{
    sqlite3* m_sqlite3;

    sqlite3_open("database.db", &m_sqlite3);

    sqlite3_close(m_sqlite3);

    //  Section: Database setup
    //-------------------------------------------------------------------------

    // Database *database = new Database();

    // database->openConnection("database.db", isSuccessful, outcomeMessage);

    // // If the operation is not successful, then just abort:
    // if(!isSuccessful)
    // {
    //     std::cout << outcomeMessage << "\n";

    //     return(0);
    // }

    //-------------------------------------------------------------------------



    //  Section: State setup
    //-------------------------------------------------------------------------

    // StateManager state_manager;

    // state_manager.run();

    //-------------------------------------------------------------------------



    //  Section: Cleanup
    //-------------------------------------------------------------------------

    // Close database:
    // database->cutConnection(isSuccessful, outcomeMessage);

    // // If the operation is not successful, then just print message:
    // if(!isSuccessful)
    // {
    //     std::cout << outcomeMessage << "\n";

    //     // TODO (SAVIZ): Not too sure what to do here to be completely honest...
    // }

    // delete database;

    //-------------------------------------------------------------------------


    return(0);
}

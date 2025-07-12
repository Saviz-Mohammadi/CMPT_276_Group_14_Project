#include <iostream>
#include "database.hpp"

Database::Database() : m_sqlite3(nullptr)
{
#ifdef DEBUG_MODE
    std::cout << "Constructor called: Database()" << "\n";
#endif
}

Database::~Database()
{
#ifdef DEBUG_MODE
    std::cout << "Destructor called: ~Database()" << "\n";
#endif
}

void Database::openConnection(const std::string &path, bool& is_successful, std::string& outcome_message)
{
    int return_code = sqlite3_open(path.c_str(), &this->m_sqlite3);

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Connection request failed: ") + sqlite3_errmsg(this->m_sqlite3);

        return;
    }

    /*
     * The { R"SQL(...)SQL" } syntax is just C++11’s raw string literal.
     *
     * 'R'     >> At the start tells: "this is a raw string".
     * '"SQL(' >> Is the opening delimiter (Can be anything). It just marks the start of the content. Everything up to the matching ')SQL"' is taken literally (no escaping of " or \ is needed).
     * ')SQL"' >> Closes the literal.
    */
    const char* sql_query = R"SQL(
        BEGIN TRANSACTION;

        -- VESSELS
        CREATE TABLE IF NOT EXISTS vessels (
            vessel_id_pk INTEGER PRIMARY KEY AUTOINCREMENT,
            vessel_name TEXT NOT NULL,
            low_ceiling_lane_length REAL NOT NULL,
            high_ceiling_lane_length REAL NOT NULL
        );

        -- SAILINGS
        CREATE TABLE IF NOT EXISTS sailings (
            sailing_id_pk INTEGER PRIMARY KEY AUTOINCREMENT,
            vessel_id_fk INTEGER NOT NULL,
            departure_terminal TEXT NOT NULL,
            departure_day INTEGER NOT NULL,
            departure_hour INTEGER NOT NULL,
            low_remaining_length REAL NOT NULL,
            high_remaining_length REAL NOT NULL,

            FOREIGN KEY(vessel_id_fk) REFERENCES vessels(vessel_id_pk)
        );

        -- VEHICLES
        CREATE TABLE IF NOT EXISTS vehicles (
            vehicle_id_pk INTEGER PRIMARY KEY AUTOINCREMENT,
            license_plate TEXT NOT NULL,
            phone_number TEXT NOT NULL,
            length REAL NOT NULL,
            height REAL NOT NULL
        );

        -- RESERVATIONS
        -- (Many-to-Many relationship between SAILINGS and VEHICLES)
        CREATE TABLE IF NOT EXISTS reservations (
            sailing_id_fk INTEGER NOT NULL,
            vehicle_id_fk INTEGER NOT NULL,
            amount_paid INTEGER NOT NULL,
            reserved_for_low_lane BOOLEAN NOT NULL,

            PRIMARY KEY (sailing_id_fk, vehicle_id_fk),
            FOREIGN KEY(sailing_id_fk) REFERENCES sailings(sailing_id_pk),
            FOREIGN KEY(vehicle_id_fk) REFERENCES vehicles(vehicle_id_pk)
        );

        COMMIT;
    )SQL";

    char* error_message = nullptr;

    return_code = sqlite3_exec(m_sqlite3, sql_query, nullptr, nullptr, &error_message);

    if (return_code != SQLITE_OK)
    {
        outcome_message = std::string("Connection request failed: ") + std::string(error_message);
        sqlite3_free(error_message);

        return;
    }

    is_successful = true;
    outcome_message = std::string("Connection request succeeded");
}

void Database::cutConnection(bool& is_successful, std::string& outcome_message)
{
    is_successful = (sqlite3_close(this->m_sqlite3) == SQLITE_OK);

    if(!is_successful)
    {
        outcome_message = std::string("Cut connection request failed: ") + sqlite3_errmsg(this->m_sqlite3);

        return;
    }

    outcome_message = std::string("Cut connection request succeeded");
}

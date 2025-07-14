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

void Database::openConnection(
    const std::string &path,
    bool& is_successful,
    std::string& outcome_message
    )
{
    int return_code = sqlite3_open(path.c_str(), &this->m_sqlite3);

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Connection request failed: ") + sqlite3_errmsg(this->m_sqlite3);

        return;
    }

    // TODO (SAVIZ): Make license_plate in vehcile a unique field.

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

    // Since we are just creating tables (no SELECT statements), we don’t need a 'callback' or 'user-data', so we pass 'nullptr' for both:
    return_code = sqlite3_exec(m_sqlite3, sql_query, nullptr, nullptr, &error_message);

    if (return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Connection request failed: ") + std::string(error_message);

        sqlite3_free(error_message);

        return;
    }

    is_successful = true;
    outcome_message = std::string("Connection request succeeded");
}

void Database::cutConnection(
    bool& is_successful,
    std::string& outcome_message
    )
{
    int return_code = sqlite3_close(this->m_sqlite3);

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Cut connection request failed: ") + sqlite3_errmsg(this->m_sqlite3);

        return;
    }

    is_successful = true;
    outcome_message = std::string("Cut connection request succeeded");
}

void Database::addVessel(
    Vessel vessel,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Creating the SQL query command:
    const char* sql_query = R"SQL(
        INSERT INTO vessels (vessel_name, low_ceiling_lane_length, high_ceiling_lane_length)
        VALUES (?, ?, ?);
    )SQL";

    // 2) Preparing the statement with bindings:
    sqlite3_stmt* prepared_sql_statement = nullptr;

    // NOTE (SAVIZ): Using version 2 (v2) instead of 'sqlite3_prepare()' for better performance and safety.
    // WARNING (SAVIZ): When using 'sqlite3_prepare_v2()' with 'nullptr' as the final parameter transactions will not work because it counts as multiple statements. If you wish to use this with multiple statements, then you need to bind to a call-back and loop thourgh it.
    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query,
        -1,                      // Tells SQLite how many bytes of your SQL buffer it should scan and compile ('-1' means read everything until the '\0' character for the end of the string)
        &prepared_sql_statement,
        nullptr                  // Useful if your buffer contains multiple SQL statements back-to-back ('nullptr' means: "I only have one statement in my string")
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Vessel creation failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    // Binding parameters (?) rather than concatenating strings (avoiding SQL-injection attacks):
    sqlite3_bind_text(
        prepared_sql_statement,
        1,                          // Place it for the first '?'
        vessel.vessel_name.c_str(),
        -1,                         // Data length in bytes ('-1' means scan until '\0')
        SQLITE_TRANSIENT            // Destructor flag: "make a copy of the text"
        );

    sqlite3_bind_double(
        prepared_sql_statement,
        2,
        vessel.low_ceiling_lane_length
        );

    sqlite3_bind_double(
        prepared_sql_statement,
        3,
        vessel.high_ceiling_lane_length
        );

    // 3) Executing:
    return_code = sqlite3_step(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Vessel creation failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        sqlite3_finalize(prepared_sql_statement);

        return;
    }

    sqlite3_int64 new_identifier = sqlite3_last_insert_rowid(m_sqlite3);

    is_successful = true;
    outcome_message = std::string("Vessel creation succeeded") + " with id of " + std::to_string(new_identifier);

    // 4) Clean up:
    sqlite3_finalize(prepared_sql_statement);
}

void Database::getVesselByID(
    int vessel_id,
    Vessel& vessel,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Creating the SQL query command:
    const char* sql_query = R"SQL(
        SELECT vessel_name, low_ceiling_lane_length, high_ceiling_lane_length FROM vessels
        WHERE vessel_id_pk = ?;
    )SQL";

    // 2) Preparing the statement with bindings:
    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Vessel get by ID failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        vessel_id
        );

    // 3) Executing:
    return_code = sqlite3_step(prepared_sql_statement);

    // Row found! Extract each column by index:
    if (return_code == SQLITE_ROW)
    {
        // NOTE (SAVIZ): We obtain every column/field in the database by calling the correct function per the type of data and correct index for the column (starting from '0'):
        vessel.vessel_id = vessel_id;

        const unsigned char* vessel_name_column_data = sqlite3_column_text(
            prepared_sql_statement,
            0);

        vessel.vessel_name = vessel_name_column_data ? reinterpret_cast<const char*>(vessel_name_column_data) : "";

        vessel.low_ceiling_lane_length = sqlite3_column_double(
            prepared_sql_statement,
            1);

        vessel.high_ceiling_lane_length = sqlite3_column_double(
            prepared_sql_statement,
            2);

        is_successful = true;
        outcome_message = std::string("Vessel get by ID succeeded.");
    }

    // Operation completed, but row was not found:
    else if (return_code == SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Vessel get by ID failed: ") + std::string("No vessel found with ID = ") + std::to_string(vessel_id);
    }

    // Operation did not complete (something has seriously gone wrong):
    else
    {
        is_successful = false;
        outcome_message = std::string("Vessel get by ID failed: ") + std::string(sqlite3_errmsg(m_sqlite3));
    }

    // 4) Clean up:
    sqlite3_finalize(prepared_sql_statement);
}

void Database::getVessels(
    int count,
    int offset,
    std::vector<Vessel>& vessels,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Creating the SQL query command:
    const char* sql_query = R"SQL(
        SELECT vessel_id_pk, vessel_name, low_ceiling_lane_length, high_ceiling_lane_length FROM vessels
        LIMIT ? OFFSET ?;
    )SQL";

    // 2) Preparing the statement with bindings:
    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Get vessels failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        count
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        offset
        );

    // 3) Executing:
    vessels.clear();

    while ((return_code = sqlite3_step(prepared_sql_statement)) == SQLITE_ROW)
    {
        Vessel vessel;

        vessel.vessel_id = sqlite3_column_int(
            prepared_sql_statement,
            0);

        const unsigned char* vessel_name_column_data = sqlite3_column_text(
            prepared_sql_statement,
            1);

        vessel.vessel_name = vessel_name_column_data ? reinterpret_cast<const char*>(vessel_name_column_data) : "";

        vessel.low_ceiling_lane_length = sqlite3_column_double(
            prepared_sql_statement,
            2);

        vessel.high_ceiling_lane_length = sqlite3_column_double(
            prepared_sql_statement,
            3);

        // TODO (SAVIZ): Might be worth to look at 'emplace_back()' for vector to increase performance:
        vessels.push_back(vessel);
    }

    // Operation completed, but row was not found:
    if (return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Get vessels failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        sqlite3_finalize(prepared_sql_statement);

        return;
    }

    is_successful = true;
    outcome_message = std::string("get Vessels succeeded.");

    // 4) Clean up:
    sqlite3_finalize(prepared_sql_statement);
}

void Database::addSailing(
    Sailing sailing,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Creating the SQL query command:
    const char* sql_query = R"SQL(
        INSERT INTO sailings (vessel_id_fk, departure_terminal, departure_day, departure_hour, low_remaining_length, high_remaining_length)
        VALUES (?, ?, ?, ?, ?, ?);
    )SQL";

    // 2) Preparing the statement with bindings:
    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Sailing creation failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.vessel_id
        );

    sqlite3_bind_text(
        prepared_sql_statement,
        2,
        sailing.departure_terminal.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        sailing.departure_day
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        4,
        sailing.departure_hour
        );

    sqlite3_bind_double(
        prepared_sql_statement,
        5,
        sailing.low_remaining_length
        );

    sqlite3_bind_double(
        prepared_sql_statement,
        6,
        sailing.high_remaining_length
        );

    // 3) Executing:
    return_code = sqlite3_step(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Sailing creation failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        sqlite3_finalize(prepared_sql_statement);

        return;
    }

    sqlite3_int64 new_identifier = sqlite3_last_insert_rowid(m_sqlite3);

    is_successful = true;
    outcome_message = std::string("Sailing creation succeeded") + " with id of " + std::to_string(new_identifier);

    // 4) Clean up:
    sqlite3_finalize(prepared_sql_statement);
}

void Database::getVehicleByID(
    std::string license_plate,
    Vehicle& vehicle,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Creating the SQL query command:
    const char* sql_query = R"SQL(
        SELECT vehicle_id_pk, license_plate, phone_number, length, height FROM vehicles
        WHERE license_plate = ?
    )SQL";

    // 2) Preparing the statement with bindings:
    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Vehicle get by ID failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_text(
        prepared_sql_statement,
        1,
        license_plate.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    // 3) Executing:
    return_code = sqlite3_step(prepared_sql_statement);

    // Row found! Extract each column by index:
    if (return_code == SQLITE_ROW)
    {
        vehicle.vehicle_id = sqlite3_column_int(
            prepared_sql_statement,
            0
            );

        const unsigned char* license_plate_data_column = sqlite3_column_text(
            prepared_sql_statement,
            1
            );

        vehicle.license_plate = license_plate_data_column ? reinterpret_cast<const char*>(license_plate_data_column) : std::string();

        const unsigned char* phone_number_data_column = sqlite3_column_text(
            prepared_sql_statement,
            2
            );

        vehicle.phone_number = phone_number_data_column ? reinterpret_cast<const char*>(phone_number_data_column) : std::string();

        vehicle.length = sqlite3_column_double(
            prepared_sql_statement,
            3
            );

        vehicle.height = sqlite3_column_double(
            prepared_sql_statement,
            4
            );

        is_successful = true;
        outcome_message = std::string("Vehicle get by ID succeeded.");
    }

    // Operation completed, but row was not found:
    else if (return_code == SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Vehicle get by ID failed: ") + std::string("No Vehicle found with license plate = ") + license_plate;
    }

    // Operation did not complete (something has seriously gone wrong):
    else
    {
        is_successful = false;
        outcome_message = std::string("Vehicle get by ID failed: ") + std::string(sqlite3_errmsg(m_sqlite3));
    }

    // 4) Clean up:
    sqlite3_finalize(prepared_sql_statement);
}

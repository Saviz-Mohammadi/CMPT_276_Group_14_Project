#include <iostream>
#include "database.hpp"

// WARNING (SAVIZ): When using 'sqlite3_prepare_v2()' with 'nullptr' as the final parameter transactions will not work because it counts as multiple statements. If you wish to use this with multiple statements, then you need to bind to a call-back and loop thourgh it.

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

    const char* sql_query = R"SQL(
        BEGIN TRANSACTION;

        -- VESSELS
        CREATE TABLE IF NOT EXISTS vessels (
            vessel_id_pk INTEGER PRIMARY KEY AUTOINCREMENT,
            vessel_name TEXT NOT NULL UNIQUE,
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

            FOREIGN KEY(vessel_id_fk) REFERENCES vessels(vessel_id_pk),

            -- Making the combination of the sailing ID unique:
            UNIQUE(departure_terminal, departure_day, departure_hour)
        );

        -- VEHICLES
        CREATE TABLE IF NOT EXISTS vehicles (
            vehicle_id_pk INTEGER PRIMARY KEY AUTOINCREMENT,
            license_plate TEXT NOT NULL UNIQUE,
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

    is_successful = true;
    outcome_message = std::string("Vessel creation succeeded");

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

    while((return_code = sqlite3_step(prepared_sql_statement)) == SQLITE_ROW)
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

    // 4.a) Operation completed, but row was not found:
    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Get vessels failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        sqlite3_finalize(prepared_sql_statement);

        return;
    }

    // 4.b) It is the responsibility of the calling code to handle cases where no records are returned based on the size of the vector.

    // 4.c) Successful:
    is_successful = true;
    outcome_message = std::string("Get vessels succeeded.");

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

    is_successful = true;
    outcome_message = std::string("Sailing creation succeeded");

    // 4) Clean up:
    sqlite3_finalize(prepared_sql_statement);
}

void Database::removeSailing(
    Sailing sailing,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Delete all reservations for this sailing
    const char* sql_query_delete_reservations = R"SQL(
        DELETE FROM reservations
        WHERE sailing_id_fk = ?;
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_delete_reservations,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Sailing deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.sailing_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Sailing deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 2) Delete the sailing record
    const char* sql_query_delete_sail = R"SQL(
        DELETE FROM sailings
        WHERE sailing_id_pk = ?;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_delete_sail,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Sailing deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.sailing_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Sailing deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 4) Success
    is_successful = true;
    outcome_message = std::string("Sailing deletion succeeded");
}

void Database::getSailingByID(
    std::string departure_terminal,
    int departure_day,
    int departure_hour,
    Sailing &sailing,
    bool &is_successful,
    std::string &outcome_message
    )
{
    // 1) Prepare the SELECT statement
    const char* sql_query = R"SQL(
        SELECT sailing_id_pk, vessel_id_fk, departure_terminal, departure_day, departure_hour, low_remaining_length, high_remaining_length FROM sailings
        WHERE departure_terminal = ? AND departure_day = ? AND departure_hour = ?
        LIMIT 1;
    )SQL";

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
        outcome_message = std::string("Get sailing by ID failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 2) Bind parameters
    sqlite3_bind_text(
        prepared_sql_statement,
        1,
        departure_terminal.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        departure_day
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        departure_hour
        );

    // 3) Execute and fetch
    return_code = sqlite3_step(prepared_sql_statement);

    if(return_code == SQLITE_ROW)
    {
        sailing.sailing_id = sqlite3_column_int(
            prepared_sql_statement,
            0
            );

        sailing.vessel_id = sqlite3_column_int(
            prepared_sql_statement,
            1
            );

        const unsigned char* departure_terminal_column_data = sqlite3_column_text(
            prepared_sql_statement,
            2
            );

        sailing.departure_terminal = departure_terminal_column_data ? reinterpret_cast<const char*>(departure_terminal_column_data) : std::string();

        sailing.departure_day = sqlite3_column_int(
            prepared_sql_statement,
            3
            );

        sailing.departure_hour = sqlite3_column_int(
            prepared_sql_statement,
            4
            );

        sailing.low_remaining_length = sqlite3_column_double(
            prepared_sql_statement,
            5
            );

        sailing.high_remaining_length = sqlite3_column_double(
            prepared_sql_statement,
            6
            );

        is_successful = true;
        outcome_message = "Get sailing by ID succeeded.";
    }

    else if(return_code == SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Get sailing by ID failed: ") + std::string("No sailing found for ID of ") + departure_terminal + "-" + std::to_string(departure_day) + "-" + std::to_string(departure_hour);
    }

    else
    {
        is_successful = false;
        outcome_message = std::string("Get sailing by ID failed: ") + sqlite3_errmsg(m_sqlite3);
    }

    // 4) Finalize
    sqlite3_finalize(prepared_sql_statement);
}

void Database::getSailingReports(
    int count,
    int offset,
    std::vector<SailingReport>& sailing_reports,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // NOTE (SAVIZ): I was planning to use the FLOOR() function in SQLite, but enabling it proved to be somewhat troublesome. So, I decided to use TRUNC() instead, as it achieves the same result (as we don't have negative values).

    // 1) Creating the SQL query command:
    const char* sql_query_sailing_reports = R"SQL(
        SELECT sailings.departure_terminal, sailings.departure_day, sailings.departure_hour, sailings.low_remaining_length, sailings.high_remaining_length, vessels.vessel_name,

        COUNT(reservations.vehicle_id_fk) AS reserved_vehicle_count,
        TRUNC
        (
            (
            (vessels.low_ceiling_lane_length + vessels.high_ceiling_lane_length - sailings.low_remaining_length - sailings.high_remaining_length) / (vessels.low_ceiling_lane_length + vessels.high_ceiling_lane_length)
            ) * 100.0
        ) AS occupancy_percentage

        FROM sailings

        JOIN vessels ON sailings.vessel_id_fk = vessels.vessel_id_pk
        LEFT JOIN reservations ON reservations.sailing_id_fk = sailings.sailing_id_pk
        GROUP BY sailings.sailing_id_pk ORDER BY sailings.departure_day, sailings.departure_hour

        LIMIT ? OFFSET ?;
    )SQL";

    // 2) Preparing the statement with bindings:
    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_sailing_reports,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Get sailing reports failed: ") + sqlite3_errmsg(m_sqlite3);

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

    // 3) Executing and populating results:
    sailing_reports.clear();

    while((return_code = sqlite3_step(prepared_sql_statement)) == SQLITE_ROW)
    {
        SailingReport sailing_report;

        const unsigned char* departure_terminal_column_data = sqlite3_column_text(
            prepared_sql_statement,
            0
            );

        sailing_report.sailing.departure_terminal = departure_terminal_column_data ? reinterpret_cast<const char*>(departure_terminal_column_data) : "";

        sailing_report.sailing.departure_day = sqlite3_column_int(
            prepared_sql_statement,
            1
            );

        sailing_report.sailing.departure_hour = sqlite3_column_int(
            prepared_sql_statement,
            2
            );

        const unsigned char* vessel_name_column_data = sqlite3_column_text(
            prepared_sql_statement,
            3
            );

        sailing_report.vessel.vessel_name = vessel_name_column_data ? reinterpret_cast<const char*>(vessel_name_column_data) : "";

        sailing_report.sailing.low_remaining_length = sqlite3_column_double(
            prepared_sql_statement,
            4
            );

        sailing_report.sailing.high_remaining_length = sqlite3_column_double(
            prepared_sql_statement,
            5
            );

        sailing_report.vehicle_count = sqlite3_column_int(
            prepared_sql_statement,
            6
            );

        sailing_report.occupancy_percentage = sqlite3_column_double(
            prepared_sql_statement,
            7
            );

        // TODO (SAVIZ): Might be worth to look at 'emplace_back()' for vector to increase performance:
        sailing_reports.push_back(sailing_report);
    }

    // 4.a) Check for errors in stepping:
    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Get sailing reports failed: ") + sqlite3_errmsg(m_sqlite3);

        sqlite3_finalize(prepared_sql_statement);

        return;
    }

    // 4.b) Success
    is_successful = true;
    outcome_message = std::string("Get sailing reports succeeded");

    sqlite3_finalize(prepared_sql_statement);
}

void Database::getSailingReportByID(
    Sailing sailing,
    SailingReport& sailing_report,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // NOTE (SAVIZ): I was planning to use the FLOOR() function in SQLite, but enabling it proved to be somewhat troublesome. So, I decided to use TRUNC() instead, as it achieves the same result (as we don't have negative values).

    // 1) Creating the SQL query command:
    const char* sql_query_sailing_report = R"SQL(
        SELECT sailings.departure_terminal, sailings.departure_day, sailings.departure_hour, sailings.low_remaining_length, sailings.high_remaining_length, vessels.vessel_name,

        COUNT(reservations.vehicle_id_fk) AS reserved_vehicle_count,
        TRUNC
        (
            (
            (vessels.low_ceiling_lane_length + vessels.high_ceiling_lane_length - sailings.low_remaining_length - sailings.high_remaining_length) / (vessels.low_ceiling_lane_length + vessels.high_ceiling_lane_length)
            ) * 100.0
        ) AS occupancy_percentage

        FROM sailings

        JOIN vessels ON sailings.vessel_id_fk = vessels.vessel_id_pk
        LEFT JOIN reservations ON reservations.sailing_id_fk = sailings.sailing_id_pk

        WHERE sailings.departure_terminal = ? AND sailings.departure_day = ? AND sailings.departure_hour = ?

        GROUP BY sailings.sailing_id_pk;
    )SQL";

    // 2) Prepare statement:
    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_sailing_report,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Get sailing report by ID failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    // 3) Bind key parameters:
    sqlite3_bind_text(
        prepared_sql_statement,
        1,
        sailing.departure_terminal.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        sailing.departure_day
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        sailing.departure_hour
        );

    // 4) Execute:
    return_code = sqlite3_step(prepared_sql_statement);

    if(return_code == SQLITE_ROW)
    {
        const unsigned char* departure_terminal_column_data = sqlite3_column_text(prepared_sql_statement, 0);

        sailing_report.sailing.departure_terminal = departure_terminal_column_data ? reinterpret_cast<const char*>(departure_terminal_column_data) : "";

        sailing_report.sailing.departure_day = sqlite3_column_int(
            prepared_sql_statement,
            1
            );

        sailing_report.sailing.departure_hour = sqlite3_column_int(
            prepared_sql_statement,
            2
            );

        sailing_report.sailing.low_remaining_length = sqlite3_column_double(
            prepared_sql_statement,
            3
            );

        sailing_report.sailing.high_remaining_length = sqlite3_column_double(
            prepared_sql_statement,
            4
            );

        const unsigned char* vessel_name_column_data = sqlite3_column_text(prepared_sql_statement, 5);

        sailing_report.vessel.vessel_name = vessel_name_column_data ? reinterpret_cast<const char*>(vessel_name_column_data) : "";

        sailing_report.vehicle_count = sqlite3_column_int(
            prepared_sql_statement,
            6
            );

        sailing_report.occupancy_percentage = sqlite3_column_double(
            prepared_sql_statement,
            7
            );

        is_successful = true;
        outcome_message = std::string("Get sailing report by ID succeeded");
    }

    // 4.b) No matching sailing
    else if(return_code == SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Get sailing report by ID failed: ") + std::string("No sailing found for given ID.");
    }

    // 4.c) Some other error
    else
    {
        is_successful = false;
        outcome_message = std::string("Get sailing report by ID failed: ") + std::string(sqlite3_errmsg(m_sqlite3));
    }

    sqlite3_finalize(prepared_sql_statement);
}

void Database::addReservation(
    Sailing sailing,
    Vehicle vehicle,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Check space and choose a lane:
    double new_low_remaining_length = sailing.low_remaining_length;
    double new_high_remaining_length = sailing.high_remaining_length;
    bool reserved_for_low_lane = false;

    if(vehicle.length <= sailing.low_remaining_length && vehicle.height <= 2)
    {
        reserved_for_low_lane = true;
        new_low_remaining_length = sailing.low_remaining_length - vehicle.length - 0.5;
    }

    else if(vehicle.length <= sailing.high_remaining_length)
    {
        reserved_for_low_lane = false;
        new_high_remaining_length = sailing.high_remaining_length - vehicle.length - 0.5;
    }

    else
    {
        is_successful = false;
        outcome_message = std::string("Reservation creation failed: ") + std::string("not enough space on sailing.");

        return;
    }

    // 2) Update sailings table to deduct the vehicle’s length:
    const char* sql_query_update_sailing = R"SQL(
        UPDATE sailings SET low_remaining_length = ?, high_remaining_length = ?
        WHERE sailing_id_pk = ?;
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_update_sailing,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Reservation creation failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_double(
        prepared_sql_statement,
        1,
        new_low_remaining_length
        );

    sqlite3_bind_double(
        prepared_sql_statement,
        2,
        new_high_remaining_length
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        sailing.sailing_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Reservation creation failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 3) Insert reservation record:
    const char* sql_query_insert_reservation = R"SQL(
        INSERT INTO reservations (sailing_id_fk, vehicle_id_fk, amount_paid, reserved_for_low_lane)
        VALUES (?, ?, ?, ?);
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_insert_reservation,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Reservation creation failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        vehicle.vehicle_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        0 // NOTE (SAVIZ): When making a reservation, the starting amount should always be '0'.
        );

    // NOTE (SAVIZ): There simply isn’t a 'sqlite3_bind_bool()' in the API. Booleans in SQL are just integers:
    sqlite3_bind_int(
        prepared_sql_statement,
        4,
        reserved_for_low_lane ? 1 : 0
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Reservation creation failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    // 4) Success
    is_successful = true;
    outcome_message = std::string("Reservation creation succeeded");
}

void Database::removeReservation(
    Sailing sailing,
    Vehicle vehicle,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Check 'reserved_for_low_lane' flag before deleting
    const char* sql_query_check = R"SQL(
        SELECT reserved_for_low_lane FROM reservations
        WHERE sailing_id_fk = ? AND vehicle_id_fk = ?
        LIMIT 1;
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_check,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        vehicle.vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    int reserved_low = 0;

    if (return_code == SQLITE_ROW)
    {
        reserved_low = sqlite3_column_int(
            prepared_sql_statement,
            0
            );
    }

    else
    {
        sqlite3_finalize(prepared_sql_statement);

        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + std::string("reservation not found!");

        return;
    }

    sqlite3_finalize(prepared_sql_statement);

    // 2) Delete the reservation record
    const char* sql_query_delete_reservation = R"SQL(
        DELETE FROM reservations
        WHERE sailing_id_fk = ? AND vehicle_id_fk = ?;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_delete_reservation,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        vehicle.vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 3) Return length to sailing lanes:
    double amount = vehicle.length + 0.5;

    const char* sql_query_update_sailing = nullptr;

    if (reserved_low)
    {
        sql_query_update_sailing = "UPDATE sailings SET low_remaining_length = low_remaining_length + ? WHERE sailing_id_pk = ?;";
    }

    else
    {
        sql_query_update_sailing = "UPDATE sailings SET high_remaining_length = high_remaining_length + ? WHERE sailing_id_pk = ?;";
    }

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_update_sailing,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_double(
        prepared_sql_statement,
        1,
        amount
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        sailing.sailing_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 4) Success
    is_successful = true;
    outcome_message = std::string("Reservation deletion succeeded: ") + "returned length = " + std::to_string(amount);
}

void Database::completeBoarding(
    Sailing sailing,
    Vehicle vehicle,
    bool &is_successful,
    std::string &outcome_message
    )
{
    // 1) Check reservation exists:
    const char* sql_query_check = R"SQL(
        SELECT 1 FROM reservations
        WHERE sailing_id_fk = ? AND vehicle_id_fk = ?
        LIMIT 1;
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_check,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Boarding failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        vehicle.vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_ROW)
    {
        is_successful = false;
        outcome_message = std::string("Boarding failed: ") + std::string("No reserevation found.");

        return;
    }

    // 2) Check if vehicle is already boarded:
    const char* sql_query_paid = R"SQL(
        SELECT amount_paid FROM reservations
        WHERE sailing_id_fk = ? AND vehicle_id_fk = ?
        LIMIT 1;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_paid,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Boarding failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing.sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        vehicle.vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    if(return_code == SQLITE_ROW)
    {
        double already_paid = sqlite3_column_double(
            prepared_sql_statement,
            0
            );

        sqlite3_finalize(prepared_sql_statement);

        if(already_paid != 0.0)
        {
            is_successful = false;
            outcome_message = std::string("Boarding failed: ") + std::string("vehicle has already been boarded (amount_paid = ") + std::to_string(already_paid) + ").";

            return;
        }
    }

    else
    {
        // This really shouldn’t happen, since we just saw the row, but check anyway:
        sqlite3_finalize(prepared_sql_statement);

        is_successful = false;
        outcome_message = std::string("Boarding failed: ") + std::string("could not verify prior payment status.");

        return;
    }

    // 3) Calculate amount to be paid:
    double amount = 0.0;

    bool is_not_tall_and_not_long = false;

    // Long vehicles pay $2 per meter:
    if(vehicle.length > 7.0)
    {
        amount += vehicle.length * 2.0;
        is_not_tall_and_not_long = true;
    }

    // Tall vehicles pay $3 per meter:
    if(vehicle.height > 2.0)
    {
        amount += vehicle.length * 3.0;
        is_not_tall_and_not_long = true;
    }

    // Short & low vehicles pay a flat fee
    if(is_not_tall_and_not_long)
    {
        amount = 14.0;
    }

    // 4) Update amount_paid in reservations
    const char* sql_query_update = R"SQL(
        UPDATE reservations SET amount_paid = ?
        WHERE sailing_id_fk = ? AND vehicle_id_fk = ?;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_update,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful  = false;
        outcome_message = std::string("Boarding failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    sqlite3_bind_double(
        prepared_sql_statement,
        1,
        amount
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        sailing.sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        vehicle.vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Boarding failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 5) Success
    is_successful = true;
    outcome_message = std::string("Boarding complete: amount_paid = ") + std::to_string(amount);
}

void Database::addVehicle(
    Vehicle vehicle,
    int& vehicle_id,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Prepare INSERT INTO vehicles
    const char* sql_query_add_vehicle = R"SQL(
        INSERT INTO vehicles (license_plate, phone_number, length, height)
        VALUES (?, ?, ?, ?);
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_add_vehicle,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Vehicle creation failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 2) Bind parameters: license_plate, phone_number, length
    sqlite3_bind_text(
        prepared_sql_statement,
        1,
        vehicle.license_plate.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    sqlite3_bind_text(
        prepared_sql_statement,
        2,
        vehicle.phone_number.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    sqlite3_bind_double(
        prepared_sql_statement,
        3,
        vehicle.length
        );

    sqlite3_bind_double(
        prepared_sql_statement,
        4,
        vehicle.height
        );

    // 3) Execute insertion
    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if (return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Vehicle creation failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 4) Retrieve new vehicle ID
    vehicle_id = static_cast<int>(sqlite3_last_insert_rowid(m_sqlite3));

    is_successful = true;
    outcome_message = std::string("Vehicle created succeeded");
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
        outcome_message = std::string("Get vehicle by ID failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

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
        outcome_message = std::string("Get vehicle by ID succeeded.");
    }

    // Operation completed, but row was not found:
    else if (return_code == SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Get vehicle by failed: ") + std::string("No vehicle found with license plate = ") + license_plate;
    }

    // Operation did not complete (something has seriously gone wrong):
    else
    {
        is_successful = false;
        outcome_message = std::string("Get vehicle by failed: ") + std::string(sqlite3_errmsg(m_sqlite3));
    }

    // 4) Clean up:
    sqlite3_finalize(prepared_sql_statement);
}

// TODO: Make a function called getSailingByID


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

            FOREIGN KEY(vessel_id_fk) REFERENCES vessels(vessel_id_pk)
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
    // TODO: Check if it exists (vessel_id_fk, departure_terminal, departure_day, departure_hour)
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

// TODO: We're assuming that the two functions of getSailingReportsByID and getVehicleByID are called before this function and provided with the correct parameters
void Database::addReservation(
    Sailing sailing,
    Vehicle vehicle,
    bool& is_successful,
    std::string& outcome_message
    )
{
    // 1) Check that the sailing exists:
    // TODO: Check if you can try using getSailingReportsByID instead of creating a new query
    const char* sql_query_check_sailing = R"SQL(
        SELECT 1 FROM sailings
        WHERE departure_terminal = ? AND departure_day = ? AND departure_hour = ?
        LIMIT 1;
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_check_sailing,
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

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_ROW)
    {
        is_successful = false;
        outcome_message = std::string("Reservation creation failed: ") + "Sailing not found for ID of " + sailing.departure_terminal + "-" + std::to_string(sailing.departure_day) + "-" + std::to_string(sailing.departure_hour);

        return;
    }

    // 1) Try to find existing vehicle by license plate:
    const char* sql_query_find_vehicle = R"SQL(
        SELECT vehicle_id_pk FROM vehicles
        WHERE license_plate = ?
        LIMIT 1;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_find_vehicle,
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

    sqlite3_bind_text(
        prepared_sql_statement,
        1,
        vehicle.license_plate.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    return_code = sqlite3_step(prepared_sql_statement);

    int vehicle_id_pk = 0;

    if(return_code == SQLITE_ROW)
    {
        vehicle_id_pk = sqlite3_column_int(
            prepared_sql_statement,
            0
            );
    }

    sqlite3_finalize(prepared_sql_statement);

    // 2) If not found, insert new vehicle:
    if(vehicle_id_pk == 0)
    {
        const char* sql_query_insert_vehicle = R"SQL(
            INSERT INTO vehicles (license_plate, phone_number, length, height)
            VALUES (?, ?, ?, ?);
        )SQL";

        return_code = sqlite3_prepare_v2(
            m_sqlite3,
            sql_query_insert_vehicle,
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

        return_code = sqlite3_step(prepared_sql_statement);

        sqlite3_finalize(prepared_sql_statement);

        if (return_code != SQLITE_DONE)
        {
            is_successful = false;
            outcome_message = std::string("Reservation creation failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

            return;
        }

        vehicle_id_pk = static_cast<int>(sqlite3_last_insert_rowid(m_sqlite3));
    }

    // 3) Insert reservation record:

    // TODO (SAVIZ): I need to do two things here: 1- Calculate where the vehicle should go and assign the correct place for low or high 2- Decrement the remaining space (For both of these I need help from the group)
    // First check if the low lane is full or not, if it is full and the vehicle is not tall, place it in the high lane, if it is, all tall vehicles can only be put in the high lane. Otherwise short vehicles all go into the low lane first
    // Tall vehicles are >= 2 meters
    // If you put a short vehicle into the high lane there is a bool on the reservation record that you need to set
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
        vehicle_id_pk
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        0 // TODO (SAVIZ): How do we calculate the amount paid?
        // Amount paid can be calculated inside the completeBoarding menu state 
        );

    // NOTE (SAVIZ): There simply isn’t a 'sqlite3_bind_bool()' in the API. Booleans in SQL are just integers:
    sqlite3_bind_int(
        prepared_sql_statement,
        4,
        (vehicle.length <= sailing.low_remaining_length) ? 1 : 0 // TODO (SAVIZ): I need to ask someone how we decide this.
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

//TODO: Call getSailingReportsByID and getVehicleByID before calling this function
void Database::removeReservation(
    std::string departure_terminal,
    int departure_day,
    int departure_hour,
    std::string license_plate,
    bool &is_successful,
    std::string &outcome_message
    )
{
    // 1) Lookup sailing_id by terminal/day/hour
    const char* sql_query_check_sailing = R"SQL(
        SELECT sailing_id_pk FROM sailings
        WHERE departure_terminal = ? AND departure_day = ? AND departure_hour = ?
        LIMIT 1;
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_check_sailing,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

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

    return_code = sqlite3_step(prepared_sql_statement);

    int sailing_id = 0;

    if(return_code == SQLITE_ROW)
    {
        sailing_id = sqlite3_column_int(
            prepared_sql_statement,
            0
            );
    }

    sqlite3_finalize(prepared_sql_statement);

    if(sailing_id == 0)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + "Sailing not found for ID of " + departure_terminal + "-" + std::to_string(departure_day) + "-" + std::to_string(departure_hour);

        return;
    }

    // 2) Lookup vehicle_id by license_plate
    const char* sql_query_find_vehicle = R"SQL(
        SELECT vehicle_id_pk FROM vehicles
        WHERE license_plate = ?
        LIMIT 1;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_find_vehicle,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_text(
        prepared_sql_statement,
        1,
        license_plate.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    return_code = sqlite3_step(prepared_sql_statement);

    int vehicle_id = 0;

    if(return_code == SQLITE_ROW)
    {
        vehicle_id = sqlite3_column_int(
            prepared_sql_statement,
            0
            );
    }

    sqlite3_finalize(prepared_sql_statement);

    if(vehicle_id == 0)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + std::string("No vehicle found with license_plate = ") + license_plate;

        return;
    }

    // 3) Delete the reservation
    const char* sql_query_delete_reservation = R"SQL(
        DELETE FROM reservations
        WHERE sailing_id_fk = ?
        AND vehicle_id_fk = ?;
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
        outcome_message = std::string("Reservation deletion failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    sqlite3_bind_int(
        prepared_sql_statement,
        1,
        sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + std::string(sqlite3_errmsg(m_sqlite3));

        return;
    }

    // 4) Check that a row was actually deleted
    int changes = sqlite3_changes(m_sqlite3);

    if(changes == 0)
    {
        is_successful = false;
        outcome_message = std::string("Reservation deletion failed: ") + std::string("No reservation found to delete for sailing ID = ") + departure_terminal + "-" + std::to_string(departure_day) + "-" + std::to_string(departure_hour) + " and vehicle license plate = " + license_plate;

        return;
    }

    // TODO (SAVIZ): I need to also find a way to increment the sailing remaining length.
    // Note: Every vehicle subtracts an extra 0.5 meters from the remainign length whenever it is added to a sailing, when removing a reservation, make sure to account for this

    // 5) Success
    is_successful = true;
    outcome_message = std::string("Reservation creation succeeded");
}

void Database::completeBoarding(
    std::string departure_terminal,
    int departure_day,
    int departure_hour,
    std::string license_plate,
    bool &is_successful,
    std::string &outcome_message
    )
{
    // 1) Lookup sailing_id, remaining lengths by terminal/day/hour
    const char* sql_query_find_sailing = R"SQL(
        SELECT sailing_id_pk, low_remaining_length, high_remaining_length FROM sailings
        WHERE departure_terminal = ? AND departure_day = ? AND departure_hour = ?
        LIMIT 1;
    )SQL";

    sqlite3_stmt* prepared_sql_statement = nullptr;

    int return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_find_sailing,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful   = false;
        outcome_message = std::string("Boarding failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

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

    return_code = sqlite3_step(prepared_sql_statement);

    int sailing_id = 0;
    double low_remaining = 0.0;
    double high_remaining = 0.0;

    if(return_code == SQLITE_ROW)
    {
        sailing_id = sqlite3_column_int(
            prepared_sql_statement,
            0
            );

        low_remaining = sqlite3_column_double(
            prepared_sql_statement,
            1
            );

        high_remaining= sqlite3_column_double(
            prepared_sql_statement,
            2
            );
    }

    sqlite3_finalize(prepared_sql_statement);

    if(sailing_id == 0)
    {
        is_successful   = false;
        outcome_message = std::string("Boarding failed: ") + "Sailing not found for " + departure_terminal + "-" + std::to_string(departure_day) + "-" + std::to_string(departure_hour);

        return;
    }

    // 2) Lookup vehicle_id and length by license_plate
    const char* sql_query_find_vehicle = R"SQL(
        SELECT vehicle_id_pk, length FROM vehicles
        WHERE license_plate = ?
        LIMIT 1;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_find_vehicle,
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
    sqlite3_bind_text(
        prepared_sql_statement,
        1,
        license_plate.c_str(),
        -1,
        SQLITE_TRANSIENT
        );

    return_code = sqlite3_step(prepared_sql_statement);

    int vehicle_id = 0;
    double vehicle_length = 0.0;

    if(return_code == SQLITE_ROW)
    {
        vehicle_id = sqlite3_column_int(
            prepared_sql_statement,
            0
            );

        vehicle_length = sqlite3_column_double(
            prepared_sql_statement,
            1
            );
    }

    sqlite3_finalize(prepared_sql_statement);

    if(vehicle_id == 0)
    {
        is_successful = false;
        outcome_message = std::string("Boarding failed: ") + std::string("No vehicle found with plate = ") + license_plate;

        return;
    }

    // 3) Lookup reserved_for_low_lane flag
    const char* sql_query_check_reservation = R"SQL(
        SELECT reserved_for_low_lane FROM reservations
        WHERE sailing_id_fk = ? AND vehicle_id_fk = ?
        LIMIT 1;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_check_reservation,
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
        sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    int reserved_low = 0;

    if(return_code == SQLITE_ROW)
    {
        reserved_low = sqlite3_column_int(
            prepared_sql_statement,
            0
            );
    }

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_ROW)
    {
        is_successful = false;
        outcome_message = std::string("Boarding failed: ") + "No reservation found.";

        return;
    }

    // 3) Update reservation amount_paid:
    const char* sql_query_update_res = R"SQL(
        UPDATE reservations SET amount_paid = ?
        WHERE sailing_id_fk = ? AND vehicle_id_fk = ?;
    )SQL";

    return_code = sqlite3_prepare_v2(
        m_sqlite3,
        sql_query_update_res,
        -1,
        &prepared_sql_statement,
        nullptr
        );

    if(return_code != SQLITE_OK)
    {
        is_successful= false;
        outcome_message = std::string("Boarding failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    double amount = 100.0; // TODO: replace with real calculation or parameter
    // Note: We are now doing this in the completeBoarding menu state

    sqlite3_bind_double(
        prepared_sql_statement,
        1,
        amount
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        2,
        sailing_id
        );

    sqlite3_bind_int(
        prepared_sql_statement,
        3,
        vehicle_id
        );

    return_code = sqlite3_step(prepared_sql_statement);

    sqlite3_finalize(prepared_sql_statement);

    if(return_code != SQLITE_DONE)
    {
        is_successful   = false;
        outcome_message = std::string("Boarding failed: ") + sqlite3_errmsg(m_sqlite3);

        return;
    }

    // 4) Success
    is_successful = true;
    outcome_message = std::string("Boarding succeeded");
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

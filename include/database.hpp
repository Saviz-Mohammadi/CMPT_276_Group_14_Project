// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Database Module
 *
 *
 * [FILE NAME]
 *
 * database.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a centralized location for orchestrating all database related functionality.
*/

// ============================================================================
// ============================================================================

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <string>
#include <vector>
#include "containers.hpp"

class Database
{
public:
    // ----------------------------------------------------------------------------
    explicit Database();

    /*
    *   [Description]
    *   Constructor for the Database class, used to instantiate a physical object in memory.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    ~Database();

    /*
    *   [Description]
    *   Destructor for the Database class, responsible for deallocating the object from memory.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------

public:
    // DONE
    // ----------------------------------------------------------------------------
    void openConnection(
        const std::string& path,     // [IN]  | The path to the database file where the connection is to be established.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to establish a connection to an SQLite database file.
    *   If a file already exists at the specified path, it connects to that file.
    *   Otherwise, it creates a new file from scratch and initializes it with the appropriate schema.
    *   It is important to call this function before any other database-related function, as the others rely on an active database connection and will not work if one has not been established.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid path>
    *       If the input path is missing, incomplete, or invalid, the operation will terminate with a failure status and provide an appropriate error message saying "Invalid path!".
    *   @ <Connection already exists>
    *       If the method has already been called and a connection exists, the operation will terminate with a failure status and provide an appropriate error message saying "Connection already exists!".
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void cutConnection(
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to close the SQLite database connection to the file.
    *   It is important to call this function before closing the program to ensure all resources are freed.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Connection does not exist>
    *       If this method is called without an existing connection, the operation will terminate with a failure status and provide an appropriate error message for diagnosis.
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void addVessel(
        Vessel vessel,               // [IN]  | Data for the new vessel that will be created.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to create and insert a new vessel into the database using SQL queries.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid vessel>
    *       If the input vessel is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid vessel!".
    *   @ <Vessel already exists>
    *       If a vessel with the exact provided data already exists, the operation will terminate with a failure status and provide an appropriate error message saying "Record already exists!".
    *   @ <Not enough space>
    *       If the database ever runs out of space (an unlikely scenario in this case), the operation will terminate with a failure status and provide an appropriate error message for diagnosis.
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void getVesselByID(
        int vessel_id,               // [IN]  | The ID of vessel to be searched for.
        Vessel& vessel,              // [OUT] | The vessel that is found.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to search for and retrieve a vessel by its ID using SQL queries.
    *   It is useful for verifying the existence of a vessel before performing other operations.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Empty table>
    *       If the database contains no records in the vessel table, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    *   @ <Invalid ID>
    *       If a vessel with the exact provided ID does not exist in the database, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void getVessels(
        int count,                    // [IN]  | The number of vessels to be retrieved.
        int offset,                   // [IN]  | Determines the starting point the retrieve query.
        std::vector<Vessel>& vessels, // [OUT] | The list of vessels that were retrieved.
        bool& is_successful,          // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message  // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to retrieve a list of vessels (usually of length '5') using SQL queries.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Empty table>
    *       If the database contains no records in the vessel table, the operation will terminate with a failure status and provide an appropriate error message saying "No records available!".
    *   @ <Empty list due to offset>
    *       If the given offset results in no remaining vessels in the table and the returned list is empty, the operation will terminate with a failure status and provide an appropriate error message saying "Empty list!".
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void addSailing(
        Sailing sailing,             // [IN]  | Data for the new sailing that will be created.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to create and insert a new sailing into the database using SQL queries.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid sailing>
    *       If the input sailing is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid sailing!".
    *       The check for the validity of the 'vessel_id' used in the sailing will be already conducted in the state layer via the 'getVesselByID()' invokation.
    *   @ <Invalid date>
    *       If the input sailing includes a date that is not within the next 28 days, the operation will terminate with a failure status and provide an appropriate error message saying "Invalid date!".
    *   @ <Sailing already exists>
    *       If a sailing with the exact provided data already exists, the operation will terminate with a failure status and provide an appropriate error message saying "Record already exists!".
    *   @ <Not enough space>
    *       If the database ever runs out of space (an unlikely scenario in this case), the operation will terminate with a failure status and provide an appropriate error message for diagnosis.
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void removeSailing(
        Sailing sailing,             // [IN]  | The sailing being targeted for deletion.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to delete a sailing from the database using SQL queries.
    *   Note that this method will also delete any associated reservations as a side effect.
    *   Note that it is assumed that 'getSailingByID()' will be successfully called before this.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid sailing>
    *       If the input sailing is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid sailing!".
    *   @ <Sailing does not exist>
    *       If a sailing with the exact provided data does not exist, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void getSailingByID(
        std::string departure_terminal, // [IN]  | The departure terminal of the sailing in the form of 3 characters.
        int departure_day,              // [IN]  | The departure day of the sailing in the form of 2 digits.
        int departure_hour,             // [IN]  | The departure hour of the sailing in the form of 2 digits.
        Sailing& sailing,               // [OUT] | The sailing object data to be stored.
        bool& is_successful,            // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message    // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to search for and retrieve a sailing by its ID using SQL queries.
    *   It is useful for verifying the existence of a sailing before performing other operations.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Empty table>
    *       If the database contains no records in the sailing table, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    *   @ <Invalid ID>
    *       If a sailing with the exact provided ID does not exist in the database, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    */
    // ----------------------------------------------------------------------------



    // NEED HELP
    // ----------------------------------------------------------------------------
    void getSailingReports(
        int count,                                   // [IN]  | The number of sailing reports to be retrieved.
        int offset,                                  // [IN]  | Determines the starting point the retrieve query.
        std::vector<SailingReport>& sailing_reports, // [OUT] | The list of sailing reports that were retrieved.
        bool& is_successful,                         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message                 // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to retrieve a list of sailing reports (usually of length '5') using SQL queries.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Empty tables>
    *       If the database contains no records for sailing report to be generated, the operation will terminate with a failure status and provide an appropriate error message saying "No records available!".
    *   @ <Empty list due to offset>
    *       If the given offset results in no remaining sailing reports in the table and the returned list is empty, the operation will terminate with a failure status and provide an appropriate error message saying "Empty list!".
    */
    // ----------------------------------------------------------------------------



    // NEED HELP
    // ----------------------------------------------------------------------------
    void getSailingReportByID(
        std::string departure_terminal, // [IN]  | The departure terminal of the sailing in the form of 3 characters.
        int departure_day,              // [IN]  | The departure day of the sailing in the form of 2 digits.
        int departure_hour,             // [IN]  | The departure hour of the sailing in the form of 2 digits.
        SailingReport& sailing_report,  // [OUT] | The list of sailing reports that were retrieved.
        bool& is_successful,            // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message    // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to retrieve a sailing report by ID using SQL queries.
    *   Note that a sailing ID is composed of three elements: departure terminal, departure day, and departure hour.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Empty tables>
    *       If the database contains no records for sailing report to be generated, the operation will terminate with a failure status and provide an appropriate error message saying "No records available!".
    */
    // ----------------------------------------------------------------------------



    // NEED HELP
    // ----------------------------------------------------------------------------
    void addReservation(
        Sailing sailing,             // [IN]  | The sailing that the new reservation will be associated to.
        Vehicle vehicle,             // [IN]  | Data about the vehicle of the reservation.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to create a reservation by using SQL queries.
    *   Note that it is assumed that 'getVehicleByID()' and 'getSailingByID()' are successfully called before this.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid sailing>
    *       If the input sailing is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid sailing!".
    *   @ <Invalid vehicle>
    *       If the input vehicle is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid vehicle!".
    *   @ <Reservation already exists>
    *       If a reservation with the exact provided data already exists, the operation will terminate with a failure status and provide an appropriate error message saying "Record already exists!".
    *   @ <Not enough space>
    *       If the database ever runs out of space (an unlikely scenario in this case), the operation will terminate with a failure status and provide an appropriate error message for diagnosis.
    */
    // ----------------------------------------------------------------------------



    // NEED HELP
    // ----------------------------------------------------------------------------
    void removeReservation(
        Sailing sailing,             // [IN]  | The sailing that the reservation is associated to.
        Vehicle vehicle,             // [IN]  | The vehicle that the reservation is associated to.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to delete a reservation by using SQL queries.
    *   Note that it is assumed that 'getVehicleByID()' and 'getSailingByID()' are successfully called before this.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid sailing>
    *       If the input sailing is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid sailing!".
    *   @ <Invalid vehicle>
    *       If the input vehicle is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid vehicle!".
    *   @ <Reservation does not exist>
    *       If a reservation with the exact provided data does not exist, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    */
    // ----------------------------------------------------------------------------



    // NEED HELP
    // ----------------------------------------------------------------------------
    void completeBoarding(
        Sailing sailing,             // [IN]  | The sailing that the reservation is associated to for completing the boarding.
        Vehicle vehicle,             // [IN]  | The vehicle that the reservation is associated to for completing the boarding.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to complete the boarding for a reservation by using SQL queries.
    *   Note that it is assumed that 'getVehicleByID()' and 'getSailingByID()' are successfully called before this.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid ID>
    *       If an invalid sailing ID is provided, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    *   @ <Invalid license plate>
    *       If an invalid license plate is provided, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void addVehicle(
        Vehicle vehicle,             // [IN]  | Data for the new vehicle that will be created.
        int& vehicle_id,             // [OUT] | The returned ID of the new vehicle.
        bool& is_successful,         // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to create and insert a new vehicle into the database using SQL queries.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid vehicle>
    *       If the input vehicle is missing, incomplete, or invalid (an unlikely scenario as validation is made in the input layer), the operation will terminate with a failure status and provide an appropriate error message saying "Invalid vehicle!".
    *   @ <Vehicle already exists>
    *       If a vehicle with the exact provided data already exists, the operation will terminate with a failure status and provide an appropriate error message saying "Record already exists!".
    *   @ <Not enough space>
    *       If the database ever runs out of space (an unlikely scenario in this case), the operation will terminate with a failure status and provide an appropriate error message for diagnosis.
    */
    // ----------------------------------------------------------------------------



    // DONE
    // ----------------------------------------------------------------------------
    void getVehicleByID(
        std::string license_plate,      // [IN]  | The license plate of the vehicle targeted for search and retrieval.
        Vehicle& vehicle,               // [OUT] | The retrieved vehicle data.
        bool& is_successful,            // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
        std::string& outcome_message    // [OUT] | A descriptive message explaining the result of the operation.
        );

    /*
    *   [Description]
    *   This function attempts to search and retrieve a vehicle with a license plate (ID) by using SQL queries.
    *   It is important to call 'openConnection()' before invoking this method.
    *
    *   [Return]
    *   void
    *
    *   [Errors]
    *   @ <Invalid license plate>
    *       If an invalid license plate is provided, the operation will terminate with a failure status and provide an appropriate error message saying "Record does not exist!".
    *   @ <Empty tables>
    *       If the database contains no records for vehicles, the operation will terminate with a failure status and provide an appropriate error message saying "No records available!".
    */
    // ----------------------------------------------------------------------------

private:
    // The SQLite connection entity. (Used as a means to interact with the underlying database)
    sqlite3* m_sqlite3;
};

#endif // DATABASE_HPP

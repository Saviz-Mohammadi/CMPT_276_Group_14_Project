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
 * Rev 1 - 2025/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer/07
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

#include <string>
#include <sqlite3.h>
#include <vector>
#include "vessel.hpp"
#include "sailing.hpp"
#include "vehicle.hpp"

class Database
{
public:
    explicit Database();
    ~Database();

public:

    // ----------------------------------------------------------------------------
    /*
       Description: Opens a connection to the SQLite database

    */
    /*
    DEVELOPER COMMENT: Takes in path as a string, opens SQLite database connection, returns status bool to confirm whether or not it is successful,
    outcome_message is the message describing the status, THIS FUNCTION MUST BE CALLED BEFORE EVERYTHING ELSE.
    Example successful outcome message: "Successfully connected to the database"
    */
    void openConnection(const std::string& path, bool& is_successful, std::string& outcome_message);


    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENT: Destroys the existing connection, all resources will be freed for the file system.
    
    */
    void cutConnection(bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
        Description: Adds a vessel to the database, upon completion it will return status and outcome message
        Return: void
        Exceptions: Not a vessel object, unitialized vessel object, vessel object already exists
    */
    void addVessel( 
        Vessel vessel, // [IN] Vessel object
        bool& is_successful, //  [OUT] Indicates if operation was successful
        std::string& outcome_message // [OUT] Status message of the operation
    );
    /*
    DEVELOPER COMMENT: Takes in a vessel id, searches it then returns corresponding vessel in the vessel reference
    */
    void getVesselByID(int id, Vessel& vessel, bool& is_successful, std::string& outcome_message); 
    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENT: Takes in count and offset, returns count vessels starting from offset, outcome_message says "nothing to retrieve"
    if the returned vessels vector has 0 length
    */
    void getVessels(int count, int offset, std::vector<Vessel>& vessels, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENT: Takes in a sailing object, checks if it already exists, and it is within 28 days. Returns respective
    status bool and outcome message
    */
    void createSailing(Sailing sailing, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENTS: Takes in a sailing objective, checks if it exists, returns status bool and outcome message
    */
    void deleteSailing(Sailing sailing, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /* 
    DEVELOPER COMMENT: Taking in count and offset, returns count sailings at offset in sailings reference
    */
    void getSailingReports(int count, int offset, std::vector<SailingReport>& sailing_reports, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENT: takes in 3 parts of the sailing id then look up in database then gets back sailing report
    */
    void getSailingReportByID(std::string departure_terminal, int departure_day, int departure_hour, SailingReport& sailing_report, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENT: takes in sailing id, vehicle, then creates reservation. If vehicle exist then it creates one, also changes remaining length of sailing
    */
    void makeReservation(Sailing sailing, Vehicle vehicle, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
    DEVLOPER COMMENT: takes in sailing id, license plate, looks up in reservation table, cancels if it exists, then changes remaining length of sailing
    */
    void cancelReservation(std::string departure_terminal, int departure_day, int departure_hour, std::string license_plate, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENT: takes in sailing id, license plate, looks up reservation if it exists then boards them. If reservation doesnt exist then you can like prompt back to make reservation or something
    */
    void completeBoarding(std::string departure_terminal, int departure_day, int departure_hour, std::string license_plate, bool& is_successful, std::string& outcome_message);
    // ----------------------------------------------------------------------------
    /*
    DEVELOPER COMMENT: takes in license plate then looks up the vehicle, returns if it exists
    */
    void getVehicleByID(std::string license_plate, Vehicle& vehicle, bool& is_successful, std::string& outcome_message)
private:
    sqlite3* m_sqlite3;
};

#endif // DATABASE_HPP

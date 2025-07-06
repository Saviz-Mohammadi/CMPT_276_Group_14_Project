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
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
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
    void openConnection(const std::string& path, bool& is_successful, std::string& outcome_message);
    void cutConnection(bool& is_successful, std::string& outcome_message);
    void addVessel(Vessel vessel, bool& is_successful, std::string& outcome_message);
    void getVessels(int count, int offset, std::vector<Vessel>& vessels, bool& is_successful, std::string& outcome_message);
    void createSailing(Sailing sailing, bool& is_successful, std::string& outcome_message);
    void deleteSailing(Sailing sailing, bool& is_successful, std::string& outcome_message);
    void getSailings(int count, int offset, std::vector<Sailing>& sailings, std::vector<Vessel>& vessels, bool& is_successful, std::string& outcome_message);
    void getSailing(std::string departure_terminal, int departure_day, int departure_hour, Sailing& sailing, Vessel& vessel, bool& is_successful, std::string& outcome_message);
    void makeReservation(Sailing sailing, Vehicle vehicle, bool& is_successful, std::string& outcome_message);
    void cancelReservation(std::string departure_terminal, int departure_day, int departure_hour, std::string license_plate, bool& is_successful, std::string& outcome_message);
    void completeBoarding(std::string departure_terminal, int departure_day, int departure_hour, std::string license_plate, bool& is_successful, std::string& outcome_message);
private:
    sqlite3* m_sqlite3;
};

#endif // DATABASE_HPP

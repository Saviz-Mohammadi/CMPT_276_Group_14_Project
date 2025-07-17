// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Container Module
 *
 *
 * [FILE NAME]
 *
 * containers.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file defines a set of struct containers, each corresponding to an entity in the database.
 * This is useful for transferring data and providing a clear representation of each entity within the project.
*/

// ============================================================================
// ============================================================================

#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <string>

struct Vessel
{
public:
    explicit Vessel();
    explicit Vessel(const int vessel_id, const std::string& vessel_name, const double low_ceiling_lane_length, const double high_ceiling_lane_length);
    ~Vessel();

public:
    int vessel_id;
    std::string vessel_name;
    double low_ceiling_lane_length;
    double high_ceiling_lane_length;
};

struct Sailing
{
public:
    explicit Sailing();
    explicit Sailing(const int sailing_id, const int vessel_id, const std::string& departure_terminal, const int departure_day, const int departure_hour, const double low_remaining_length, const double high_remaining_length);
    ~Sailing();

public:
    int sailing_id;
    int vessel_id;
    std::string departure_terminal;
    int departure_day;
    int departure_hour;
    double low_remaining_length;
    double high_remaining_length;
};

struct Reservation
{
public:
    explicit Reservation();
    explicit Reservation(const int sailing_id, const int vehicle_id, const int amount_paid, const bool reserved_for_low_lane);
    ~Reservation();

public:
    int sailing_id;
    int vehicle_id;
    int amount_paid;
    bool reserved_for_low_lane;
};

struct Vehicle
{
public:
    explicit Vehicle();
    explicit Vehicle(const int vehicle_id, const std::string& license_plate, const std::string& phone_number, const double length, const double height);
    ~Vehicle();

public:
    int vehicle_id;
    std::string license_plate;
    std::string phone_number;
    double length;
    double height;
};

struct SailingReport
{
public:
    explicit SailingReport();
    explicit SailingReport(const Sailing& sailing, const Vessel& vessel, const int vehicle_count, const double occupancy_percentage);
    ~SailingReport();

public:
    Sailing sailing; 
    Vessel vessel;
    int vehicle_count;
    double occupancy_percentage;
};

#endif // CONTAINERS_HPP

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
 * vehicle.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a raw data container for a vehicle entity.
*/

// ============================================================================
// ============================================================================

#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>

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

#endif // VEHICLE_HPP

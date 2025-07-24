// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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
 * Rev 1 - 2025/07/05 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 * 
 *
 * [PURPOSE]
 *
 * This file defines a set of struct containers, each corresponding to an entity in 
 * the database.
 * This is useful for transferring data and providing a clear representation of each
 * entity within the project.
*/

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <string>

// ----------------------------------------------------------------------------
struct Vessel
{
public:
    explicit Vessel();
    explicit Vessel(
        const int vessel_id, 
        const std::string& vessel_name, 
        const double low_ceiling_lane_length, 
        const double high_ceiling_lane_length);
    ~Vessel();

public:
    int vessel_id;                      //unique autoincrementing integer key
    std::string vessel_name;            //<=25 characters string name of the vessel, unique
    double low_ceiling_lane_length;     //total combined length in meters of all low lanes
    double high_ceiling_lane_length;    //total combined length in meters of all high lanes
};

// ----------------------------------------------------------------------------
struct Sailing
{
public:
    explicit Sailing();
    explicit Sailing(
        const int sailing_id, 
        const int vessel_id, 
        const std::string& departure_terminal, 
        const int departure_day, 
        const int departure_hour, 
        const double low_remaining_length, 
        const double high_remaining_length
    );
    ~Sailing();

public:
    int sailing_id;                     //unique autoincrementing integer key
    int vessel_id;                      //foreign key from vessel table
    std::string departure_terminal;     //TTT part of sailing id string
    int departure_day;                  //DD part of sailing id string
    int departure_hour;                 //HH part of sailing id string 0-24
    double low_remaining_length;        //total combined length in meters of free space
                                        //in low lanes
    double high_remaining_length;       //total combined length in meters of free space
                                        //in high lanes
};

// ----------------------------------------------------------------------------
struct Reservation
{
public:
    explicit Reservation();
    explicit Reservation(
        const int sailing_id, 
        const int vehicle_id, 
        const int amount_paid, 
        const bool reserved_for_low_lane
    );
    ~Reservation();

public:
    int sailing_id;                 //foreign key from sailing table
    int vehicle_id;                 //foreign key from sailing table
    int amount_paid;                //price of fare in cents, 0 or -1 if not boarded
    bool reserved_for_low_lane;     //needed to check if a short vehicle is in a tall lane
};

// ----------------------------------------------------------------------------
struct Vehicle
{
public:
    explicit Vehicle();
    explicit Vehicle(
        const int vehicle_id,
        const std::string& license_plate, 
        const std::string& phone_number, 
        const double length, 
        const double height);
    ~Vehicle();

public:
    int vehicle_id;             //unique autoincrementing integer key
    std::string license_plate;  //license plate or the vehicle, 2-10 characters
    std::string phone_number;   //owner phone number, 8-14 characters
    double length;              //in meters
    double height;              //in meters
};

// ----------------------------------------------------------------------------
// Contains all neccessary information for all columns in the sailing report
//
struct SailingReport
{
public:
    explicit SailingReport();
    explicit SailingReport(
        const Sailing& sailing, 
        const Vessel& vessel, 
        const int vehicle_count, 
        const double occupancy_percentage
    );
    ~SailingReport();

public:
    Sailing sailing;                //the sailing object
    Vessel vessel;                  //the vessel object
    int vehicle_count;              //total number of vehicles reserved or boarded on the sailing
    double occupancy_percentage;    //percentages are fractions of 1. e.g. 98% = 0.98
};

#endif // CONTAINERS_HPP

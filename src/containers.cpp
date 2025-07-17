#include "containers.hpp"

Vessel::Vessel() :
    vessel_id(0),
    vessel_name(),
    low_ceiling_lane_length(0.0),
    high_ceiling_lane_length(0.0)
{

}

Vessel::Vessel(
    const int vessel_id,
    const std::string& vessel_name,
    const double low_ceiling_lane_length,
    const double high_ceiling_lane_length) :
    vessel_id(vessel_id),
    vessel_name(vessel_name),
    low_ceiling_lane_length(low_ceiling_lane_length),
    high_ceiling_lane_length(high_ceiling_lane_length)
{
}

Vessel::~Vessel() = default;

Sailing::Sailing() :
    sailing_id(0),
    vessel_id(0),
    departure_terminal(),
    departure_day(0),
    departure_hour(0),
    low_remaining_length(0.0),
    high_remaining_length(0.0)
{
}

Sailing::Sailing(
    const int sailing_id,
    const int vessel_id,
    const std::string& departure_terminal,
    const int departure_day,
    const int departure_hour,
    const double low_remaining_length,
    const double high_remaining_length) :
    sailing_id(sailing_id),
    vessel_id(vessel_id),
    departure_terminal(departure_terminal),
    departure_day(departure_day),
    departure_hour(departure_hour),
    low_remaining_length(low_remaining_length),
    high_remaining_length(high_remaining_length)
{
}

Sailing::~Sailing() = default;

Reservation::Reservation():
    sailing_id(0),
    vehicle_id(0),
    amount_paid(0),
    reserved_for_low_lane(false)
{   
}

Reservation::Reservation(
    const int sailing_id,
    const int vehicle_id,
    const int amount_paid,
    const bool reserved_for_low_lane) :
    sailing_id(sailing_id),
    vehicle_id(vehicle_id),
    amount_paid(amount_paid),
    reserved_for_low_lane(reserved_for_low_lane)
{
}

Reservation::~Reservation()= default;

Vehicle::Vehicle() :
    vehicle_id(0),
    license_plate(),
    phone_number(),
    length(0.0),
    height(0.0)
{
}

Vehicle::Vehicle(
    const int vehicle_id,
    const std::string& license_plate,
    const std::string& phone_number,
    const double length,
    const double height) :
    vehicle_id(vehicle_id),
    license_plate(license_plate),
    phone_number(phone_number),
    length(length),
    height(height)
{
}

Vehicle::~Vehicle() = default;

SailingReport::SailingReport() :
    sailing(),
    vessel(),
    vehicle_count(0),
    occupancy_percentage(0.0)
{
}

SailingReport::SailingReport(
    const Sailing& sailing,
    const Vessel& vessel,
    const int vehicle_count,
    const double occupancy_percentage) :
    sailing(sailing),
    vessel(vessel),
    vehicle_count(vehicle_count),
    occupancy_percentage(occupancy_percentage)
{
}

SailingReport::~SailingReport() = default;

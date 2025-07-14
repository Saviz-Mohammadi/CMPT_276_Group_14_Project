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

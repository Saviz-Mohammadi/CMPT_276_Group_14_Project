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

Vessel::~Vessel()
{
}

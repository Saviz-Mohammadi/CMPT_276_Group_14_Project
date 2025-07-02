#ifndef VESSEL_HPP
#define VESSEL_HPP

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

#endif // VESSEL_HPP

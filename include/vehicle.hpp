#ifndef VEHICLE_H
#define VEHICLE_H

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

#endif // VEHICLE_H

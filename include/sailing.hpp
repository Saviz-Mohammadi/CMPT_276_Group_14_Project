#ifndef SAILING_H
#define SAILING_H

#include <string>

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

#endif // SAILING_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "utilities.hpp"

void Utilities::extractSailingID(std::string& sailing_id, std::string& terminal, int& departure_day, int& departure_hour) {
    // String stream
    std::stringstream string_stream(sailing_id);

    // Other variables
    char dummy; 

    std::getline(string_stream, terminal, '-'); // Get string and put in terminal up until first dash
    string_stream >> departure_day >> dummy >> departure_hour; // Uses dummy to consume the 2nd dash
}

void Utilities::createSailingID(const std::string& terminal, const int departure_day, const int departure_hour, std::string& output_sailing_id)
{
    std::ostringstream oss;
    oss << terminal
        << '-'
        << std::setw(2) << std::setfill('0') << departure_day
        << '-'
        << std::setw(2) << std::setfill('0') << departure_hour;
    output_sailing_id = oss.str();
}

bool Utilities::almostEqual(
    double first_number,
    double second_number,
    double epsilon
    )
{
    return(std::abs(first_number - second_number) < epsilon);
}

std::string Utilities::getLocalDateAndTime()
{
    std::time_t time = std::time(
        nullptr
        );

    std::tm tm = *std::localtime(
        &time
        );

    std::ostringstream output_stream;

    output_stream << std::put_time(&tm, "%Y-%m-%d  %H:%M:%S");

    return(output_stream.str());
}

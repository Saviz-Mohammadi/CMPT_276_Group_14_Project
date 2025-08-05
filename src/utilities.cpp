// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
 * utilities.cpp
 *
 * [REVISION HISTORY]
 *
 * Rev 1 – 2025/07/23 Original by Henry, Saviz Mohammadi
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  - Sailing IDs are strings in the form "Terminal-DD-HH".
 *  - Local date/time is represented via std::time_t and std::tm.
 *
 * Memory / speed / complexity trade‑offs:
 *  - Uses `std::stringstream`/`std::ostringstream` for clarity; incurs extra allocations.
 *  - Formatting with `std::setw` and `std::setfill` guarantees two‑digit fields.
 *
 * Parsing & formatting abstraction:
 *  - `extractSailingID` centralizes splitting a sailing‑ID into terminal, day, hour.
 *  - `createSailingID` centralizes zero‑padded composition of a sailing‑ID.
 *
 * Floating‑point comparison:
 *  - `almostEqual` uses `std::abs` and an epsilon to avoid strict equality pitfalls.
 *
 * Date/time retrieval:
 *  - `getLocalDateAndTime` uses `std::localtime` + `std::put_time` for ISO‑style timestamps.
 *  - Note: `std::localtime` is not thread‑safe; consider `std::localtime_r` for multithreading.
 *
 * Error handling & assumptions:
 *  - All functions assume well‑formed input; no validation or error reporting is performed.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "utilities.hpp"

// ****************************************************************************
void Utilities::extractSailingID(std::string& sailing_id, std::string& terminal, int& departure_day, int& departure_hour)
{
    // String stream
    std::stringstream string_stream(sailing_id);

    // Other variables
    char dummy; 

    std::getline(string_stream, terminal, '-'); // Get string and put in terminal up until first dash
    string_stream >> departure_day >> dummy >> departure_hour; // Uses dummy to consume the 2nd dash
}

// ****************************************************************************
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

// ****************************************************************************
bool Utilities::almostEqual(
    double first_number,
    double second_number,
    double epsilon
    )
{
    return(std::abs(first_number - second_number) < epsilon);
}

// ****************************************************************************
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

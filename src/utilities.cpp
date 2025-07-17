
#include <iostream>
#include <string>
#include <sstream>
#include "utilities.hpp"


Utilities::extractSailingID(std::string& sailing_id, std::string& terminal, int& departure_day, int& departure_hour) {
    // String stream
    std::stringstream string_stream(sailing_id);

    // Other variables
    char dummy; 

    std::getline(string_stream, terminal, '-'); // Get string and put in terminal up until first dash
    string_stream >> departure_day >> dummy >> departure_hour; // Uses dummy to consume the 2nd dash
}

Utilities::createSailingID(const std::string& terminal, const int departure_day, const int departure_hour, std::string& output_sailing_id) {

    output_sailing_id = std::format("{}-{:02}-{:02}", terminal, departure_day, departure_hour)

}
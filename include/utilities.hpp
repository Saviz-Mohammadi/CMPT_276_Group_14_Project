// TODO: Add more comments here
// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Utilities Mdule
 *
 *
 * [FILE NAME]
 *
 * utilities.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/7 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a variety of helper functions that are consistently used in other parts of the program
*/

// ============================================================================
// ============================================================================

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <ctime>
#include "containers.hpp"


namespace Utilities
{
    std::string getLocalDateAndTime();

    void extractSailingID(
        std::string& sailing_id, 
        std::string& terminal, 
        int& departure_day, 
        int& departure_hour
        ); 

    void createSailingID(
        const std::string& terminal, 
        const int departure_day, 
        const int departure_hour,
        std::string& output_sailing_id
        );

    bool almostEqual(
        double first_number,
        double second_number,
        double epsilon = std::numeric_limits<double>::epsilon()
        );
}

#endif // UTILITIES_HPP

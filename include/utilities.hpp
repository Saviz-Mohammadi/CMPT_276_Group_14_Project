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
#include "containers.hpp"


class Utilities
{
public:
    // ----------------------------------------------------------------------------
    explicit Utilities();

    /*
    *   [Description]
    *   Constructor for the Utilities class, used to instantiate a physical object in memory.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------



    // ----------------------------------------------------------------------------
    ~Utilities();

    /*
    *   [Description]
    *   Destructor for the Utilities class, responsible for deallocating the object from memory.
    *
    *   [Return]
    *   N/A
    *
    *   [Errors]
    *   N/A
    */
    // ----------------------------------------------------------------------------

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

}

#endif // UTILITIES_HPP
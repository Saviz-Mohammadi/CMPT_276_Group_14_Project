// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*
 * [MODULE]
 *
 * Test Module
 *
 *
 * [FILE NAME]
 *
 * test_utilities.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/20 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 * 
 * Rev 2 - 2025/??/??
 *       - Ethan. Added case for a sailing ids with all digits as zero
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for testing utility functionality.
*/

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include <catch2/catch_all.hpp>
#include "utilities.hpp"

// ----------------------------------------------------------------------------
TEST_CASE("Extract sailing ID: Testing to make sure that the 'extractSailingID()' function works correctly", "[Utilities]")
{
    // Creating variables
    // ****************************************************************************

    std::string sailing_id = "ABC-12-34";
    std::string departure_terminal;
    int departure_day;
    int departure_hour;


    // Extracting
    // ****************************************************************************

    Utilities::extractSailingID(
        sailing_id,
        departure_terminal,
        departure_day,
        departure_hour
        );


    // Checking
    // ****************************************************************************

    REQUIRE(departure_terminal == "ABC");
    REQUIRE(departure_day == 12);
    REQUIRE(departure_hour == 34);
}

// ----------------------------------------------------------------------------
TEST_CASE("Extract sailing ID 2", "[Utilities]")
{
    // Creating variables
    // ****************************************************************************

    std::string sailing_id = "ABC-00-00";
    std::string departure_terminal;
    int departure_day;
    int departure_hour;


    // Extracting
    // ****************************************************************************

    Utilities::extractSailingID(
        sailing_id,
        departure_terminal,
        departure_day,
        departure_hour
    );


    // Checking
    // ****************************************************************************

    REQUIRE(departure_terminal == "ABC");
    REQUIRE(departure_day == 0);
    REQUIRE(departure_hour == 0);
}

// ----------------------------------------------------------------------------
TEST_CASE("Create sailing ID: Testing to make sure that the 'createSailingID()' function works correctly", "[Utilities]")
{
    // Creating variables
    // ****************************************************************************

    std::string sailing_id = "";


    // Extracting
    // ****************************************************************************

    Utilities::createSailingID(
        "CCC",
        9,
        20,
        sailing_id
        );


    // Checking
    // ****************************************************************************

    REQUIRE(sailing_id == "CCC-09-20");
}

// ----------------------------------------------------------------------------
TEST_CASE("Create sailing ID 2", "[Utilities]")
{
    // Creating variables
    // ****************************************************************************

    std::string sailing_id = "";


    // Extracting
    // ****************************************************************************

    Utilities::createSailingID(
        "CCC",
        0,
        0,
        sailing_id
    );


    // Checking
    // ****************************************************************************

    REQUIRE(sailing_id == "CCC-00-00");
}

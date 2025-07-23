#include <catch2/catch_all.hpp>
#include "utilities.hpp"

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

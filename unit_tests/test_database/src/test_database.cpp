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
 * test_database.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/20 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for testing database related functionality.
*/

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include <catch2/catch_all.hpp>
#include <iostream>
#include <filesystem>
#include "database.hpp"
#include "utilities.hpp"

// ----------------------------------------------------------------------------
TEST_CASE("Add vessel: Testing to make sure that the 'addVessel()' function works correctly", "[Vessel]")
{

    std::cout << "\n-----Add Vessel Test-----\n";
    bool is_successful = false;
    std::string outcome_message = "";


    //  Database setup
    // ****************************************************************************

    Database database;

    database.openConnection(
        "vessel_test.db",
        is_successful,
        outcome_message
        );

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);


    //  Making new vessel and checking it was created correctly
    // ****************************************************************************

    Vessel new_vessel;

    new_vessel.vessel_name = "Royal Bismarck";
    new_vessel.low_ceiling_lane_length = 500;
    new_vessel.high_ceiling_lane_length = 300;

    int new_vessel_id = -1;

    database.addVessel(
        new_vessel,
        new_vessel_id,
        is_successful,
        outcome_message
        );

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);

    Vessel retrieved_vessel;

    database.getVesselByID(
        new_vessel_id,
        retrieved_vessel,
        is_successful,
        outcome_message
        );

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);

    REQUIRE(retrieved_vessel.vessel_name == new_vessel.vessel_name);
    REQUIRE(retrieved_vessel.low_ceiling_lane_length == new_vessel.low_ceiling_lane_length);
    REQUIRE(retrieved_vessel.high_ceiling_lane_length == new_vessel.high_ceiling_lane_length);


    // Attempt to create the same vessel again (This should return failure as a result)
    // ****************************************************************************

    database.addVessel(
        new_vessel,
        new_vessel_id,
        is_successful,
        outcome_message
        );

    std::cout << outcome_message << "\n";

    REQUIRE(!is_successful);


    // Cleanup and Reset
    // ****************************************************************************

    database.cutConnection(is_successful, outcome_message);

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);

    std::filesystem::path path_to_file = "./vessel_test.db";

    if(std::filesystem::remove(path_to_file))
    {
        std::cout << "Deleted successfully\n";
    }

    else
    {
        std::cout << "File did not exist\n";
    }

    std::cout << "-----End Add Vessel Test-----\n";
}

TEST_CASE("Add Sailing: Test to make sure that 'addSailing()' works correctly", "[Sailing]")
{
    std::cout << "\n-----Add Sailing Test-----\n";

    const std::filesystem::path path_to_file = "./add_sailing_unit_test.db";
    const std::string file_name = "add_sailing_unit_test.db";

    bool is_successful = false;
    std::string outcome_message = "";

    
    // ****************************************************************************

    INFO("Creating database for the test");
    Database database;

    // remove the .db if it exists
    if (std::filesystem::remove(path_to_file))
    {
        std::cout << "Deleted db file successfully\n";
    }

    else
    {
        std::cout << "db file did not exist\n";
    }

    //create .db file
    database.openConnection(
        file_name,
        is_successful,
        outcome_message
    );

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);


    // ****************************************************************************

    INFO("Creating the vessel for the sailing");
    Vessel vessel(-1, "addSailing Test Vessel", 120, 85);
    int vessel_id_out = -1;

    database.addVessel(vessel, vessel_id_out, is_successful, outcome_message);

    vessel.vessel_id - vessel_id_out;

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);


    // ****************************************************************************    
    INFO("Creating the sailing record");
    Sailing sailing(-1, vessel.vessel_id, "ABC", 6, 12, vessel.low_ceiling_lane_length, vessel.high_ceiling_lane_length);
    database.addSailing(sailing, is_successful, outcome_message);
    std::cout << outcome_message << "\n";
    REQUIRE(is_successful);


    // ****************************************************************************    
    INFO("Fetching the sailing record");

    Sailing sailing_out;

    database.getSailingByID(
        sailing.departure_terminal,
        sailing.departure_day,
        sailing.departure_hour,
        sailing_out,
        is_successful,
        outcome_message
    );

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);


    // ****************************************************************************
    INFO("Check fetched record has correct information");

    REQUIRE(sailing_out.vessel_id == sailing.vessel_id);
    REQUIRE(sailing_out.departure_terminal == sailing.departure_terminal);
    REQUIRE(sailing_out.departure_day == sailing.departure_day);
    REQUIRE(sailing_out.departure_hour == sailing.departure_hour);
    REQUIRE(sailing_out.low_remaining_length == sailing.low_remaining_length);
    REQUIRE(sailing_out.high_remaining_length == sailing.high_remaining_length);


    // ****************************************************************************
    INFO("Cleanup");

    database.cutConnection(is_successful, outcome_message);

    std::cout << outcome_message << "\n";

    REQUIRE(is_successful);

    std::cout << "-----End Add Sailing Test-----\n";
}

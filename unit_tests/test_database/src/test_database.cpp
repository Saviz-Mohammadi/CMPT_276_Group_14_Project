#include <catch2/catch_all.hpp>
#include <iostream>
#include <filesystem>
#include "database.hpp"
#include "utilities.hpp"

TEST_CASE("Add vessel: Testing to make sure that the 'addVessel()' function works correctly", "[Vessel]")
{
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

    std::cout << outcome_message << "\n\n";

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

    std::cout << outcome_message << "\n\n";

    REQUIRE(is_successful);

    Vessel retrieved_vessel;

    database.getVesselByID(
        new_vessel_id,
        retrieved_vessel,
        is_successful,
        outcome_message
        );

    std::cout << outcome_message << "\n\n";

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

    std::cout << outcome_message << "\n\n";

    REQUIRE(!is_successful);


    // Cleanup and Reset
    // ****************************************************************************

    database.cutConnection(is_successful, outcome_message);

    std::cout << outcome_message << "\n\n";

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
}

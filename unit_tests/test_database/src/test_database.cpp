#include <catch2/catch_all.hpp>
#include <iostream>
#include "database.hpp"
#include "utilities.hpp"

TEST_CASE("Add vessel: Testing to make sure that the 'addVessel()' function works correctly", "[Vessel]")
{
    //  Section: Database setup
    // ****************************************************************************

    bool is_successful = false;
    std::string outcome_message = "";

    Database database;

    database.openConnection("database.db", is_successful, outcome_message);

    // If the operation is not successful, then just abort:
    REQUIRE(is_successful);


    //  Section: Make new vessel and check it was created correctly
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

    // Get the vessel back from database and see if it has the correct value:
    Vessel retrieved_vessel;

    database.getVesselByID(new_vessel_id, retrieved_vessel, is_successful, outcome_message);

    std::cout << outcome_message << "\n\n";

    REQUIRE(is_successful);

    REQUIRE(retrieved_vessel.vessel_name == new_vessel.vessel_name);
    REQUIRE(retrieved_vessel.low_ceiling_lane_length == new_vessel.low_ceiling_lane_length);
    REQUIRE(retrieved_vessel.high_ceiling_lane_length == new_vessel.high_ceiling_lane_length);


    //  Section: Cleanup
    // ****************************************************************************

    database.cutConnection(is_successful, outcome_message);

    // If the operation is not successful, then just print message:
    REQUIRE(is_successful);
}

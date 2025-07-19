#include <catch2/catch_all.hpp>
#include "utilities.hpp"
#include "containers.hpp"

TEST_CASE("Vehicle: default constructor initializes to zeros and empty strings", "[Vehicle]")
{
    Vehicle vehicle;

    REQUIRE(vehicle.vehicle_id == 0);
    REQUIRE(vehicle.license_plate == "");
    REQUIRE(vehicle.phone_number == "");
    REQUIRE(Utilities::almostEqual(vehicle.length, 0.0));
    REQUIRE(Utilities::almostEqual(vehicle.height, 0.0));
}

TEST_CASE("Vehicle: parameterized constructor sets all members", "[Vehicle]")
{
    Vehicle vehicle(
        42,
        "ABC-123",
        "555-6789",
        33.5,
        12.3
        );

    REQUIRE(vehicle.vehicle_id == 42);
    REQUIRE(vehicle.license_plate == "ABC-123");
    REQUIRE(vehicle.phone_number == "555-6789");
    REQUIRE(Utilities::almostEqual(vehicle.length, 33.5));
    REQUIRE(Utilities::almostEqual(vehicle.height, 12.3));
}

TEST_CASE("Vessel: default constructor initializes to zeros and empty string", "[Vessel]")
{
    Vessel vessel;

    REQUIRE(vessel.vessel_id == 0);
    REQUIRE(vessel.vessel_name == "");
    REQUIRE(Utilities::almostEqual(vessel.low_ceiling_lane_length, 0.0));
    REQUIRE(Utilities::almostEqual(vessel.high_ceiling_lane_length, 0.0));
}

TEST_CASE("Vessel: parameterized constructor sets all members", "[Vessel]")
{
    Vessel vessel(
        7,
        "Sea Owl",
        20.0,
        25.5
        );

    REQUIRE(vessel.vessel_id == 7);
    REQUIRE(vessel.vessel_name == "Sea Owl");
    REQUIRE(Utilities::almostEqual(vessel.low_ceiling_lane_length, 20.0));
    REQUIRE(Utilities::almostEqual(vessel.high_ceiling_lane_length, 25.5));
}

TEST_CASE("Sailing: default constructor initializes to zeros and empty terminal", "[Sailing]")
{
    Sailing sailing;

    REQUIRE(sailing.sailing_id == 0);
    REQUIRE(sailing.vessel_id == 0);
    REQUIRE(sailing.departure_terminal == "");
    REQUIRE(sailing.departure_day == 0);
    REQUIRE(sailing.departure_hour == 0);
    REQUIRE(Utilities::almostEqual(sailing.low_remaining_length, 0.0));
    REQUIRE(Utilities::almostEqual(sailing.high_remaining_length, 0.0));
}

TEST_CASE("Sailing: parameterized constructor sets all members", "[Sailing]")
{
    Sailing sailing(
        13,
        7,
        "Terminal 2",
        15,
        9,
        12.5,
        8.75
        );

    REQUIRE(sailing.sailing_id == 13);
    REQUIRE(sailing.vessel_id == 7);
    REQUIRE(sailing.departure_terminal == "Terminal 2");
    REQUIRE(sailing.departure_day == 15);
    REQUIRE(sailing.departure_hour == 9);
    REQUIRE(Utilities::almostEqual(sailing.low_remaining_length, 12.5));
    REQUIRE(Utilities::almostEqual(sailing.high_remaining_length, 8.75));
}

TEST_CASE("Reservation: default constructor initializes to zeros and false", "[Reservation]")
{
    Reservation reservation;

    REQUIRE(reservation.sailing_id == 0);
    REQUIRE(reservation.vehicle_id == 0);
    REQUIRE(reservation.amount_paid == 0);
    REQUIRE(reservation.reserved_for_low_lane == false);
}

TEST_CASE("Reservation: parameterized constructor sets all members", "[Reservation]")
{
    Reservation reservation(
        13,
        42,
        150,
        true
        );

    REQUIRE(reservation.sailing_id == 13);
    REQUIRE(reservation.vehicle_id == 42);
    REQUIRE(reservation.amount_paid  == 150);
    REQUIRE(reservation.reserved_for_low_lane == true);
}

TEST_CASE("SailingReport: default constructor initializes to default sub‑objects and zeroes", "[SailingReport]")
{
    SailingReport sailing_report;

    REQUIRE(sailing_report.sailing.sailing_id == 0);
    REQUIRE(sailing_report.sailing.vessel_id == 0);
    REQUIRE(sailing_report.sailing.departure_terminal == "");
    REQUIRE(sailing_report.sailing.departure_day == 0);
    REQUIRE(sailing_report.sailing.departure_hour == 0);
    REQUIRE(Utilities::almostEqual(sailing_report.sailing.low_remaining_length, 0.0));
    REQUIRE(Utilities::almostEqual(sailing_report.sailing.high_remaining_length, 0.0));

    REQUIRE(sailing_report.vessel.vessel_id == 0);
    REQUIRE(sailing_report.vessel.vessel_name == "");
    REQUIRE(Utilities::almostEqual(sailing_report.vessel.low_ceiling_lane_length, 0.0));
    REQUIRE(Utilities::almostEqual(sailing_report.vessel.high_ceiling_lane_length, 0.0));

    REQUIRE(sailing_report.vehicle_count == 0);
    REQUIRE(Utilities::almostEqual(sailing_report.occupancy_percentage, 0.0));
}

TEST_CASE("SailingReport: parameterized constructor sets all sub‑objects and fields", "[SailingReport]")
{
    Vehicle dummyVehicle;
    Vessel vessel(7, "Sea Hare", 30.0, 35.0);
    Sailing sailing(21, 7, "Dock A", 22, 14, 15.5, 10.25);
    int count = 5;
    double percentage = 80.0;

    SailingReport sailing_report(
        sailing,
        vessel,
        count,
        percentage
        );

    REQUIRE(sailing_report.sailing.sailing_id == 21);
    REQUIRE(sailing_report.sailing.vessel_id == 7);
    REQUIRE(sailing_report.sailing.departure_terminal == "Dock A");
    REQUIRE(sailing_report.sailing.departure_day == 22);
    REQUIRE(sailing_report.sailing.departure_hour == 14);
    REQUIRE(Utilities::almostEqual(sailing_report.sailing.low_remaining_length, 15.5));
    REQUIRE(Utilities::almostEqual(sailing_report.sailing.high_remaining_length, 10.25));

    REQUIRE(sailing_report.vessel.vessel_id == 7);
    REQUIRE(sailing_report.vessel.vessel_name == "Sea Hare");
    REQUIRE(Utilities::almostEqual(sailing_report.vessel.low_ceiling_lane_length, 30.0));
    REQUIRE(Utilities::almostEqual(sailing_report.vessel.high_ceiling_lane_length, 35.0));

    REQUIRE(sailing_report.vehicle_count == count);
    REQUIRE(Utilities::almostEqual(sailing_report.occupancy_percentage, percentage));
}

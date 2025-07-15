#include <catch2/catch_all.hpp>
#include <limits>
#include "containers.hpp"

// NOTE (SAVIZ): If you want to learn more about writing unit tests with the 'Catch2' library, check out the following resource: https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md#top

bool almost_equal(double first_number, double second_number, double epsilon = std::numeric_limits<double>::epsilon())
{
    return(std::abs(first_number - second_number) < epsilon);
}

TEST_CASE("Vehicle: default ctor initializes to zeros and empty strings", "[Vehicle]") {
    Vehicle v;
    REQUIRE(v.vehicle_id       == 0);
    REQUIRE(v.license_plate    == "");
    REQUIRE(v.phone_number     == "");
    REQUIRE(almost_equal(v.length, 0.0));
    REQUIRE(almost_equal(v.height, 0.0));
}

TEST_CASE("Vehicle: parameterized ctor sets all members", "[Vehicle]") {
    Vehicle v(42, "ABC-123", "555-6789", 33.5, 12.3);
    REQUIRE(v.vehicle_id       == 42);
    REQUIRE(v.license_plate    == "ABC-123");
    REQUIRE(v.phone_number     == "555-6789");
    REQUIRE(almost_equal(v.length, 33.5));
    REQUIRE(almost_equal(v.height, 12.3));
}

TEST_CASE("Vessel: default ctor initializes to zeros and empty name", "[Vessel]") {
    Vessel vs;
    REQUIRE(vs.vessel_id                == 0);
    REQUIRE(vs.vessel_name              == "");
    REQUIRE(almost_equal(vs.low_ceiling_lane_length, 0.0));
    REQUIRE(almost_equal(vs.high_ceiling_lane_length, 0.0));
}

TEST_CASE("Vessel: parameterized ctor sets all members", "[Vessel]") {
    Vessel vs(7, "Sea Owl", 20.0, 25.5);
    REQUIRE(vs.vessel_id                == 7);
    REQUIRE(vs.vessel_name              == "Sea Owl");
    REQUIRE(almost_equal(vs.low_ceiling_lane_length, 20.0));
    REQUIRE(almost_equal(vs.high_ceiling_lane_length, 25.5));
}

TEST_CASE("Sailing: default ctor initializes to zeros and empty terminal", "[Sailing]") {
    Sailing s;
    REQUIRE(s.sailing_id          == 0);
    REQUIRE(s.vessel_id           == 0);
    REQUIRE(s.departure_terminal  == "");
    REQUIRE(s.departure_day       == 0);
    REQUIRE(s.departure_hour      == 0);
    REQUIRE(almost_equal(s.low_remaining_length, 0.0));
    REQUIRE(almost_equal(s.high_remaining_length, 0.0));
}

TEST_CASE("Sailing: parameterized ctor sets all members", "[Sailing]") {
    Sailing s(
        13,          // sailing_id
        7,           // vessel_id
        "Terminal 2",
        15,          // departure_day
        9,           // departure_hour
        12.5,        // low_remaining_length
        8.75         // high_remaining_length
        );
    REQUIRE(s.sailing_id             == 13);
    REQUIRE(s.vessel_id              == 7);
    REQUIRE(s.departure_terminal     == "Terminal 2");
    REQUIRE(s.departure_day          == 15);
    REQUIRE(s.departure_hour         == 9);
    REQUIRE(almost_equal(s.low_remaining_length, 12.5));
    REQUIRE(almost_equal(s.high_remaining_length, 8.75));
}

TEST_CASE("Reservation: default ctor initializes to zeros and false flag", "[Reservation]") {
    Reservation r;
    REQUIRE(r.sailing_id            == 0);
    REQUIRE(r.vehicle_id            == 0);
    REQUIRE(r.amount_paid           == 0);
    REQUIRE(r.reserved_for_low_lane == false);
}

TEST_CASE("Reservation: parameterized ctor sets all members", "[Reservation]") {
    Reservation r(13, 42, 150, true);
    REQUIRE(r.sailing_id            == 13);
    REQUIRE(r.vehicle_id            == 42);
    REQUIRE(r.amount_paid           == 150);
    REQUIRE(r.reserved_for_low_lane == true);
}

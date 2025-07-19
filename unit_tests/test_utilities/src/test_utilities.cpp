#include <catch2/catch_all.hpp>
#include "utilities.hpp"

TEST_CASE("Utilities: Sailing ID functions", "[Utilities]") 
{
    std::string true_sailing_id = "ABC-12-34";
    std::string true_terminal = "ABC";
    int true_day = 12;
    int true_hour = 34;

    SECTION("Create sailing ID ABC-12-34") 
    {
        std::string sailing_id_out;
        Utilities::createSailingID(true_terminal, true_day, true_hour, sailing_id_out);
        REQUIRE(sailing_id_out == true_sailing_id);
    }

    SECTION("Parse sailing ID ABC-12-34")
    {
        std::string terminal_out;
        int day_out;
        int hour_out;
        Utilities::extractSailingID(true_sailing_id, terminal_out, day_out, hour_out);
        REQUIRE(terminal_out == true_terminal);
        REQUIRE(day_out == true_day);
        REQUIRE(hour_out == true_hour);
    }
}

TEST_CASE("Utilities: Sailing ID functions with leading zeroes", "[Utilities]")
{
    std::string true_sailing_id = "WWW-01-00";
    std::string true_terminal = "WWW";
    int true_day = 1;
    int true_hour = 0;

    SECTION("Create sailing ID WWW-01-00")
    {
        std::string sailing_id_out;
        Utilities::createSailingID(true_terminal, true_day, true_hour, sailing_id_out);
        REQUIRE(sailing_id_out == true_sailing_id);
    }

    SECTION("Parse sailing ID WWW-01-00")
    {
        std::string terminal_out;
        int day_out;
        int hour_out;
        Utilities::extractSailingID(true_sailing_id, terminal_out, day_out, hour_out);
        REQUIRE(terminal_out == true_terminal);
        REQUIRE(day_out == true_day);
        REQUIRE(hour_out == true_hour);
    }
}

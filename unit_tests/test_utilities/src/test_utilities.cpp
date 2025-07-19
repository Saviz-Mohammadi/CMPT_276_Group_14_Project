#include <catch2/catch_all.hpp>
#include "utilities.hpp"

void testCreateSailingID(std::string true_sailing_id, std::string true_terminal, int true_day, int true_hour) {
    SECTION("Create sailing ID " + true_sailing_id)
    {
        std::string sailing_id_out;
        Utilities::createSailingID(true_terminal, true_day, true_hour, sailing_id_out);
        REQUIRE(sailing_id_out == true_sailing_id);
    }
}

void testExtractSailingID(std::string true_sailing_id, std::string true_terminal, int true_day, int true_hour) {
    SECTION("Parse sailing ID " + true_sailing_id)
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

TEST_CASE("Utilities: Sailing ID functions", "[Utilities]") 
{
    testCreateSailingID("ABC-12-34", "ABC", 12, 34);
    testExtractSailingID("ABC-12-34", "ABC", 12, 34);

    testCreateSailingID("HIJ-00-00", "HIJ", 0, 0);
    testExtractSailingID("HIJ-00-00", "HIJ", 0, 0);

    testCreateSailingID("CCC-09-20", "CCC", 9, 20);
    testExtractSailingID("CCC-09-20", "CCC", 9, 20);
}
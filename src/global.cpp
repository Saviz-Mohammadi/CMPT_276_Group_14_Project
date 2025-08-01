// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*
 * [MODULE]
 *
 * Global Configuration
 *
 *
 * [FILE NAME]
 *
 * global.cpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 – 2025/07/23 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 * 
 * Rev 2 - 2025/??/??
 *       - Ethan.
 *       - Added global regex patterns for business strings
 *
 *
 * [DESIGN NOTES]
 *
 * Data representation:
 *  - Holds application‑wide default values for pagination, prompt results, and size limits.
 *
 * Memory / speed / complexity trade‑offs:
 *  - Uses static globals for direct, low‑overhead access.
 *  - Simplifies code by avoiding parameter passing, at the cost of global state.
 *
 * Usage:
 *  - `g_list_length` determines how many records to fetch/display by default.
 *  - `g_is_successful` and `g_outcome_message` communicate status from input helpers.
 *  - `g_allowed_yes_no_responses` and `g_allowed_navigation_responses` centralize valid input choices.
 *  - `g_vessel_name_length` bounds vessel name inputs.
 *  - `g_vehicle_*` variables define valid ranges for vehicle dimensions.
 *
 * Future enhancements:
 *  - Encapsulate into a configuration struct or singleton for testability.
 *  - Load defaults from a config file or environment to allow runtime customization.
 *
 */

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include "global.hpp"
#include <regex>

// how many items to return by default
int g_list_length = 5;

// for your prompt‑loop functions
bool g_is_successful = false;
std::string g_outcome_message = "";

// yes/no prompts—accept uppercase or lowercase
std::vector<char> g_allowed_yes_no_responses = { 'Y', 'y', 'N', 'n' };

// navigation prompts—Previous, Next, or Exit (case‑insensitive)
std::vector<char> g_allowed_navigation_responses = {
    'P', 'p',   // Previous
    'N', 'n',   // Next
    'E', 'e'    // Exit
};

// Vehicle size ranges
double g_vehicle_min_length = 0;
double g_vehicle_max_length = 100;
double g_vehicle_min_height = 0;
double g_vehicle_max_height = 10;

//regex patterns
std::regex g_vessel_name_regex (R"(^[\w '-]{1,25}$)"); //1-25 letters, spaces, apostrophes, hyphens
std::regex g_sailing_id_regex (R"(^[A-Z]{3}-\d\d-\d\d$)"); //TTT-HH-DD
std::regex g_license_plate_regex (R"(^[A-Z\d -]{1,10}$)"); //1-10 capital letters, numbers, spaces, hyphens
std::regex g_phone_number_regex (R"(^(?=.{8,14}$)(?:\d|\d-(?=\d))*$)"); //8-14 digits and hyphens, where hyphens must have digits on both sides

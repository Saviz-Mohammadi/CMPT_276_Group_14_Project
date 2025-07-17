#include "global.hpp"

// how many items to return by default
int g_list_length = 10;

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

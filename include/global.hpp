// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Global Module
 *
 *
 * [FILE NAME]
 *
 * global.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a centralized location for global configurations.
 * These configurations are shared and used throughout the program.
*/

// ============================================================================
// ============================================================================

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>
#include <vector>

// Specifies the default length for returned lists, such as lists of vessels or sailing reports.
extern int g_list_length;

// A default set of accepted characters for yes/no questions, intended for repeated use.
extern std::vector<char> g_allowed_yes_no_responses;

// A default set of accepted characters for navigational questions (Previous, Next, Exit), intended for repeated use.
extern std::vector<char> g_allowed_navigation_responses;

#endif // GLOBAL_H

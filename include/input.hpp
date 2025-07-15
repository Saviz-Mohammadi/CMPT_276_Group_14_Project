// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Input Module
 *
 *
 * [FILE NAME]
 *
 * input.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a centralized location for input functionality.
*/

// ============================================================================
// ============================================================================

#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include <vector>
#include <regex>

// ----------------------------------------------------------------------------
void promptForInteger(
    const std::string& input_prompt, // [IN]  | The message displayed when asking the user for input.
    int min,                         // [IN]  | The minimum acceptable integer value.
    int max,                         // [IN]  | The maximum acceptable integer value.
    int& acquired_integer,           // [OUT] | The integer entered and obtained by the user, if valid.
    bool& is_successful,             // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
    std::string& outcome_message     // [OUT] | A descriptive message explaining the result of the operation.
    );

/*
*   [Description]
*   This function prompts the user to enter an integer value.
*   It will read the input, attempt to parse it as an integer, and validates that
*   the value falls between the specified range (min-max).
*   On success or failure, provides an appropriate status and message for diagnosis.
*
*   [Return]
*   void
*
*   [Errors]
*   @ <Non-integer input>
*       If the user’s input cannot be parsed as an integer, the operation will terminate with a failure status and provide an appropriate error message saying "Invalid input!".
*   @ <Invalid range>
*       If the parsed integer is less than `min` or greater than `max`, the operation will terminate with a failure status and provide an appropriate error message saying "Input is out of range!".
*/
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
void promptForReal(
    const std::string& input_prompt, // [IN]  | The message displayed when asking the user for input.
    double min,                      // [IN]  | The minimum acceptable real value.
    double max,                      // [IN]  | The maximum acceptable real value.
    double& acquired_real,           // [OUT] | The real entered and obtained by the user, if valid.
    bool& is_successful,             // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
    std::string& outcome_message     // [OUT] | A descriptive message explaining the result of the operation.
    );

/*
*   [Description]
*   This function prompts the user to enter a real (double) value.
*   It will read the input, attempt to parse it as a double, and validates that
*   the value falls between the specified range (min-max).
*   On success or failure, provides an appropriate status and message for diagnosis.
*
*   [Return]
*   void
*
*   [Errors]
*   @ <Non-real input>
*       If the user’s input cannot be parsed as a real, the operation will terminate with a failure status and provide an appropriate error message saying "Invalid input!".
*   @ <Invalid range>
*       If the parsed real is less than `min` or greater than `max`, the operation will terminate with a failure status and provide an appropriate error message saying "Input is out of range!".
*/
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
void promptForCharacter(
    const std::string& input_prompt,          // [IN]  | The message displayed when asking the user for input.
    const std::vector<char>& allowed_options, // [IN]  | The set of allowed characters to check against.
    char& acquired_character,                 // [OUT] | The character entered and obtained by the user, if valid.
    bool& is_successful,                      // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
    std::string& outcome_message              // [OUT] | A descriptive message explaining the result of the operation.
    );

/*
*   [Description]
*   This function prompts the user to enter a character value (non-numeric).
*   It will read the input, attempt to parse it as a alphabetical character, and validates that the value belongs to the set of acceptable characters.
*   On success or failure, provides an appropriate status and message for diagnosis.
*
*   [Return]
*   void
*
*   [Errors]
*   @ <Non-character input>
*       If the user’s input cannot be parsed as a character, the operation will terminate with a failure status and provide an appropriate error message saying "Invalid input!".
*   @ <Not in set>
*       If the parsed character is not within the set of acceptable characters, the operation will terminate with a failure status and provide an appropriate error message saying "Input is not a correct choice!".
*/
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
void promptForString(
    const std::string& input_prompt, // [IN]  | The message displayed when asking the user for input.
    std::string& acquired_string,    // [OUT] | The string entered and obtained by the user.
    bool& is_successful,             // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
    std::string& outcome_message     // [OUT] | A descriptive message explaining the result of the operation.
    );

/*
*   [Description]
*   This function prompts the user to enter a line of string/text.
*   It will read the input line and provide it back to the calling segment.
*   On success or failure, provides an appropriate status and message for diagnosis.
*
*   [Return]
*   void
*
*   [Errors]
*   N/A
*/
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
void promptForString(
    const std::string& input_prompt, // [IN]  | The message displayed when asking the user for input.
    const std::regex& pattern,       // [IN]  | The pattern/format used to validate the string.
    std::string& acquired_string,    // [OUT] | The string entered and obtained by the user.
    bool& is_successful,             // [OUT] | The outcome status of the operation, indicating whether it was successful or not.
    std::string& outcome_message     // [OUT] | A descriptive message explaining the result of the operation.
    );

/*
*   [Description]
*   This function prompts the user to enter a line of string/text.
*   It will read the input line and attempt to validate it using regular expressions.
*   On success or failure, provides an appropriate status and message for diagnosis.
*
*   [Return]
*   void
*
*   [Errors]
*   @ <Bad format>
*       If the user’s input does no match the required pattern, the operation will terminate with a failure status and provide an appropriate error message saying "Input does not match required format!".
*/
// ----------------------------------------------------------------------------


// TODO (SAVIZ): I need to add comments for these later:

void continuouslyPromptForInteger(
    const std::string& input_prompt,
    int min,
    int max,
    int& acquired_integer
    );

void continuouslyPromptForReal(
    const std::string& input_prompt,
    double min,
    double max,
    double& acquired_real
    );

void continuouslyPromptForCharacter(
    const std::string& input_prompt,
    const std::vector<char>& allowed_options,
    char& acquired_character
    );

void continuouslyPromptForString(
    const std::string& input_prompt,
    std::string& acquired_string
    );

void continuouslyPromptForString(
    const std::string& input_prompt,
    const std::regex& validation_pattern,
    std::string& acquired_string
    );

#endif // INPUT_HPP

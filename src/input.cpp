#include "input.hpp"
#include <iostream>

void promptForInteger(
    const std::string& input_prompt,
    int min,
    int max,
    int& acquired_integer,
    bool& is_successful,
    std::string& outcome_message
    )
{
    std::string input_line = "";
    int input_integer = 0;

    std::cout << input_prompt;


    // Serious I/O failure checks
    // ************************************************************************

    auto old_mask = std::cin.exceptions();

    std::cin.exceptions(
        std::ios::badbit  | // Irrecoverable I/O errors
        std::ios::failbit | // Formatted extraction failures
        std::ios::eofbit    // End-Of-File (EOF)
        );

    try
    {
        std::getline(std::cin, input_line);
    }

    catch(const std::ios_base::failure& exception)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = std::string("Input failed: ") + exception.what();

        return;
    }

    // ************************************************************************


    // Extraction check
    // ************************************************************************

    std::stringstream string_stream(input_line);
    char remainder = '\0';

    // If extraction of an integer fails, or there was a remainder (like ".14" or "abc")
    if (!(string_stream >> input_integer) || (string_stream >> remainder))
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = "Input is invalid!";

        return;
    }

    // ************************************************************************


    // Range check
    // ************************************************************************

    bool is_out_of_range = input_integer < min || input_integer > max;

    if (is_out_of_range)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = "Input is out of range!";

        return;
    }

    // ************************************************************************


    std::cin.exceptions(old_mask);

    acquired_integer = input_integer;
    is_successful = true;
    outcome_message = "";
}

void promptForReal(
    const std::string& input_prompt,
    double min,
    double max,
    double& acquired_real,
    bool& is_successful,
    std::string& outcome_message
    )
{
    std::string input_line = "";
    double input_real = 0.0;

    std::cout << input_prompt;


    // Serious I/O failure checks
    // ************************************************************************

    auto old_mask = std::cin.exceptions();

    std::cin.exceptions(
        std::ios::badbit  | // Irrecoverable I/O errors
        std::ios::failbit | // Formatted extraction failures
        std::ios::eofbit    // End-Of-File (EOF)
        );

    try
    {
        std::getline(std::cin, input_line);
    }

    catch(const std::ios_base::failure& exception)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = std::string("Input failed: ") + exception.what();

        return;
    }

    // ************************************************************************


    // Extraction check
    // ************************************************************************

    std::stringstream string_stream(input_line);
    char remainder = '\0';

    // If extraction of a real fails, or there was a remainder (like ".14.57" or "abc")
    if (!(string_stream >> input_real) || (string_stream >> remainder))
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = "Input is invalid!";

        return;
    }

    // ************************************************************************


    // Range check
    // ************************************************************************

    bool is_out_of_range = input_real < min || input_real > max;

    if (is_out_of_range)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = "Input is out of range!";

        return;
    }

    // ************************************************************************


    std::cin.exceptions(old_mask);

    acquired_real = input_real;
    is_successful = true;
    outcome_message = "";
}

void promptForCharacter(
    const std::string& input_prompt,
    const std::vector<char>& allowed_options,
    char& acquired_character,
    bool& is_successful,
    std::string& outcome_message
    )
{
    std::string input_line = "";
    char input_character = '\0';

    std::cout << input_prompt;


    // Serious I/O failure checks
    // ************************************************************************

    auto old_mask = std::cin.exceptions();

    std::cin.exceptions(
        std::ios::badbit  | // Irrecoverable I/O errors
        std::ios::failbit | // Formatted extraction failures
        std::ios::eofbit    // End-Of-File (EOF)
        );

    try
    {
        std::getline(std::cin, input_line);
    }

    catch(const std::ios_base::failure& exception)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = std::string("Input failed: ") + exception.what();

        return;
    }

    // ************************************************************************


    // Extraction check
    // ************************************************************************

    std::stringstream string_stream(input_line);
    char remainder = '\0';

    // If extraction of a character fails, or there was a remainder (like "abc")
    if(!(string_stream >> input_character) || (string_stream >> remainder))
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = "Input is invalid!";

        return;
    }

    // ************************************************************************


    // Character check
    // ************************************************************************

    bool is_not_in_list = std::find(allowed_options.begin(), allowed_options.end(), input_character) == allowed_options.end();

    if(is_not_in_list)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = "Input is not a correct choice!";

        return;
    }

    // ************************************************************************


    std::cin.exceptions(old_mask);

    acquired_character = input_character;
    is_successful = true;
    outcome_message = "";
}

void promptForString(
    const std::string& input_prompt,
    std::string& acquired_string,
    bool& is_successful,
    std::string& outcome_message
    )
{
    std::string input_line = "";

    std::cout << input_prompt;


    // Serious I/O failure checks
    // ************************************************************************

    auto old_mask = std::cin.exceptions();

    std::cin.exceptions(
        std::ios::badbit  | // Irrecoverable I/O errors
        std::ios::failbit | // Formatted extraction failures
        std::ios::eofbit    // End-Of-File (EOF)
        );

    try
    {
        std::getline(std::cin, input_line);
    }

    catch(const std::ios_base::failure& exception)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = std::string("Input failed: ") + exception.what();

        return;
    }

    // ************************************************************************


    std::cin.exceptions(old_mask);

    acquired_string = input_line;
    is_successful = true;
    outcome_message = "";
}

void promptForString(
    const std::string& input_prompt,
    const std::regex& pattern,
    std::string& acquired_string,
    bool& is_successful,
    std::string& outcome_message
    )
{
    std::string input_line = "";

    std::cout << input_prompt;


    // Serious I/O failure checks
    // ************************************************************************

    auto old_mask = std::cin.exceptions();

    std::cin.exceptions(
        std::ios::badbit  | // Irrecoverable I/O errors
        std::ios::failbit | // Formatted extraction failures
        std::ios::eofbit    // End-Of-File (EOF)
        );

    try
    {
        std::getline(std::cin, input_line);
    }

    catch(const std::ios_base::failure& exception)
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = std::string("Input failed: ") + exception.what();

        return;
    }

    // ************************************************************************


    // Validate using regular expression
    // ************************************************************************

    if (!std::regex_match(input_line, pattern))
    {
        std::cin.clear();
        std::cin.exceptions(old_mask);

        is_successful = false;
        outcome_message = "Input does not match required format!";

        return;
    }

    // ************************************************************************


    std::cin.exceptions(old_mask);

    acquired_string = input_line;
    is_successful = true;
    outcome_message = "";
}

#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include <vector>
#include <regex>

void promptForInteger(const std::string& input_prompt, int min, int max, int& acquired_integer, bool& is_successful, std::string& outcome_message);
void promptForReal(const std::string& input_prompt, double min, double max, double& acquired_real, bool& is_successful, std::string& outcome_message);
void promptForCharacter(const std::string& input_prompt, const std::vector<char>& allowed_options, char& acquired_character, bool& is_successful, std::string& outcome_message);
void promptForString(const std::string& input_prompt, std::string& acquired_string, bool& is_successful, std::string& outcome_message);
void promptForString(const std::string& input_prompt, const std::regex& pattern, std::string& acquired_string, bool& is_successful, std::string& outcome_message);

#endif // INPUT_HPP

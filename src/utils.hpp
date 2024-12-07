#pragma once

/**
* @brief Splits a string into numbers
* @param line The string to split
* @param numbers The vector to store the numbers in
*/
void SplitIntoNumbers(const std::string& line, std::vector<int>& numbers);

/**
 * @brief Splits a string into large numbers
 * @param line The string to split
 * @param numbers The vector to store the numbers in
 */
void SplitIntoLargeNumbers(const std::string& line, std::vector<uint64_t>& numbers);
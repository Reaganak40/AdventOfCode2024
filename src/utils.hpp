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

// Hash function for std::tuple<int, int>
struct TupleHashInt2 {
    std::size_t operator()(const std::tuple<int, int>& t) const {
        // Combine the hash values of the two integers
        return std::hash<int>()(std::get<0>(t)) ^ (std::hash<int>()(std::get<1>(t)) << 1);
    }
};
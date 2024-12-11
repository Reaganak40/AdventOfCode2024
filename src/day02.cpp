#include "pch.h"
#include "utils.hpp"

/**
* @brief Checks if two vectors are increasing or decreasing
* @param a The first vector
* @param b The second vector
* @return True if the vectors are increasing or decreasing, false otherwise
*/
bool IsIncreasingOrDecreasing(const std::vector<int>& a, const std::vector<int>& b)
{
    // increasing
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            goto decreasing;
        }
    }
    return true;

decreasing:
    // decreasing
    int j = b.size() - 1;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[j])
        {
            return false;
        }
        j--;
    }

    return true;
}

/**
* @brief Checks if the level change between two numbers is at most 3
* @param nums The numbers to check
* @return True if the level change is at most 3, false otherwise
*/
bool GoodLevelChange(const std::vector<int>& nums)
{
    for (int i = 0; i < nums.size() - 1; i++) {
        int diff = std::abs(nums[i] - nums[i + 1]);
        if (diff == 0 || diff > 3) {
            return false;
        }
    }
    return true;
}

/**
* @brief Checks if the given numbers are safe, strictly increasing or decreasing, and no level change is more than 3
* @param nums The numbers to check
* @return True if the numbers are safe, false otherwise
*/
bool IsSafe(const std::vector<int>& nums)
{
    if (nums.size() <= 1) {
        return true;
    }

    // check if nums is the same when sorted
    std::vector<int> sorted_nums(nums);
	std::sort(sorted_nums.begin(), sorted_nums.end());
	if (IsIncreasingOrDecreasing(nums, sorted_nums))
	{
		return GoodLevelChange(nums);
	}

    return false;
}

/**
* @brief Checks if the given numbers are safe, allows the removing of a single number
* @param nums The numbers to check
* @return True if the numbers are safe, false otherwise
*/
bool IsSafe2(const std::vector<int>& nums)
{
	if (nums.size() <= 1) {
		return true;
	}

	// brute force: check each removal and if any safe return true
	for (int i = 0; i < nums.size(); i++)
	{
		std::vector<int> copy(nums);
		copy.erase(copy.begin() + i);
		if (IsSafe(copy))
		{
			return true;
		}
	}

	return false;
}

void DoDay2()
{
    std::cout << "Day 2" << std::endl;
    std::ifstream file("inputs/day2/input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
	int part1 = 0, part2 = 0;
    while (std::getline(file, line))
    {
        std::vector<int> numbers;
        SplitIntoNumbers(line, numbers);
        if (IsSafe(numbers))
        {
            part1++;
        }
		if (IsSafe2(numbers))
		{
			part2++;
		}
    }

	std::cout << "    Part 1: " << part1 << std::endl;
	std::cout << "    Part 2: " << part2 << std::endl;
}
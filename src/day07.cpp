#include "pch.h"
#include "utils.hpp"

uint64_t ConcatNumbers(uint64_t a, uint64_t b)
{
    std::string str_a = std::to_string(a);
    std::string str_b = std::to_string(b);
    return std::stoll(str_a + str_b);
}

bool ISValidCalibration(const std::vector<uint64_t>& nums, bool use_concat = false)
{
    const uint64_t test = nums[0];
    std::function<uint64_t(uint64_t, int)> do_operation = [&](uint64_t curr_sum, int idx) -> uint64_t
    {
        if (idx >= nums.size() || curr_sum > test)
        {
            return curr_sum;
        }

        uint64_t try_add = do_operation(curr_sum + nums[idx], idx + 1);
        if (try_add == test)
        {
            return test;
        }

        if (use_concat)
        {
            uint64_t try_concat = do_operation(ConcatNumbers(curr_sum, nums[idx]), idx + 1);
            if (try_concat == test)
            {
                return test;
            }
        }

        return do_operation(curr_sum * nums[idx], idx + 1);
    };

    return do_operation(nums[1], 2) == test;
}

void DoDay7()
{
    std::cout << "Day 7" << std::endl;
    std::ifstream file("inputs/day07/input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    uint64_t sum1 = 0;
    uint64_t sum2 = 0;
    while (std::getline(file, line))
    {
        std::vector<uint64_t> calibrations;

        // get test value (number before ':')
        std::string test = line.substr(0, line.find(':'));
        calibrations.push_back(std::stoull(test));

        line = line.substr(line.find(':') + 1);
        SplitIntoLargeNumbers(line, calibrations);

        if (ISValidCalibration(calibrations, false))
        {
            sum1 += calibrations[0];
        }

        if (ISValidCalibration(calibrations, true))
        {
            
            sum2 += calibrations[0];
        }
    }

    std::cout << "    Part1: " << sum1 << std::endl;
    std::cout << "    Part2: " << sum2 << std::endl;
}
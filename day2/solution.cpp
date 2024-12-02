#include "pch.h"


void SplitIntoNumbers(const std::string& line, std::vector<int>& numbers)
{
    std::string num;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (std::isdigit(line[i]))
        {
                num += line[i];
        } else {
            if (!num.empty())
            {
                numbers.push_back(std::stoi(num));
                num.clear();
            }
        }
    }

    if (!num.empty())
    {
        numbers.push_back(std::stoi(num));
    }
}

bool IsSafe(const std::vector<int>& nums)
{
    if (nums.size() <= 1) {
        return true;
    }

    bool increasing = nums[1] - nums[0] > 0;
    bool used_dampender = false;

    auto is_safe = [](bool increasing, int diff) -> bool {
        return increasing ? diff > 0 && diff <= 3 : diff < 0 && diff >= -3;
    };

    auto can_tolerate = [&](int& i) -> bool {
        
        // always works if removing the last element
        if (i == nums.size() - 2) {
            return true;
        }        

        int diff = nums[i + 2] - nums[i];
        if(is_safe(increasing, diff)) {
            return true;
        }

        // might need to update increasing if removing second element
        if (i == 0) {
            if (is_safe(nums[2] - nums[0] > 0, nums[2] - nums[0])) {
                increasing = nums[2] - nums[0] > 0;
                i++;
                return true;
            }

            // try removing the first element
            increasing = nums[2] - nums[1] > 0;
            return true;
        }

        return false;
    };


    for (int i = 0; i < nums.size() - 1; i++)
    {
        int diff = nums[i + 1] - nums[i];

        if (!is_safe(increasing, diff))
        {
            if (used_dampender || !can_tolerate(i))
            {
                return false;
            }
            used_dampender = true;
        }
    }

    return true;
}

void DoDay2()
{
    std::cout << "Day 2" << std::endl;
    std::ifstream file("day2/input2.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    int total_safe = 0;
    while (std::getline(file, line))
    {
        std::vector<int> numbers;
        SplitIntoNumbers(line, numbers);
        if (IsSafe(numbers))
        {
            total_safe++;
            std::cout << "Safe: " << line << std::endl;
        } else {
            std::cout << "Unsafe: " << line << std::endl;
        }
    }

    std::cout << "Total safe: " << total_safe << std::endl;
}
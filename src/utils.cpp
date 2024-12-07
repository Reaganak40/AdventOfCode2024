#include "pch.h"
#include "utils.hpp"

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

void SplitIntoLargeNumbers(const std::string &line, std::vector<uint64_t> &numbers)
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
                numbers.push_back(std::stoull(num));
                num.clear();
            }
        }
    }

    if (!num.empty())
    {
        numbers.push_back(std::stoull(num));
    }
}

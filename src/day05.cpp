#include "pch.h"
#include "utils.hpp"

bool IsValidUpdateSequence(const std::vector<int>& sequence, const std::unordered_map<int, std::vector<int>>& ordering)
{
    for (int i = 0; i < sequence.size() - 1; i++) {
        int key = sequence[i];

        if (ordering.find(key) == ordering.end()) {
            continue;
        }

        for (auto value : ordering.at(key)) {
            if (std::find(sequence.begin() + i, sequence.end(), value) != sequence.end()) {
                return false;
            }
        }
    }

    return true;
}

void FixSequence(std::vector<int>& sequence, const std::unordered_map<int, std::vector<int>>& ordering)
{
    for (int i = 0; i < sequence.size() - 1; i++) {
        int key = sequence[i];

        if (ordering.find(key) == ordering.end()) {
            continue;
        }

        int curr_key_index = i;
        int start = i;
        bool swapped = false;
        for (auto value : ordering.at(key)) {
            auto it = std::find(sequence.begin() + curr_key_index, sequence.end(), value);
            if (it != sequence.end()) {
                std::iter_swap(sequence.begin() + curr_key_index, it);
                curr_key_index = it - sequence.begin();
                swapped = true;
            }
        }

        if (swapped) {
            i = start - 1;
        }
    }
}

void DoDay5()
{
    std::cout << "Day 5" << std::endl;
    std::ifstream file("inputs/day05/input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::unordered_map<int, std::vector<int>> ordering;
    std::string line;
    
    // create ordering array
    // ordering[key] = [value1, value2, ...]   => all the values must come after key
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }

        // all numbers are two digits
        int num1 = std::stoi(line.substr(0, 2));
        int num2 = std::stoi(line.substr(3, 2));

        if (ordering.find(num2) == ordering.end()) {
            ordering[num2] = std::vector<int>();
        }
        ordering.at(num2).push_back(num1);
    }

    // check if each update sequence is valid
    int sum1 = 0;
    int sum2 = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<int> numbers;
        SplitIntoNumbers(line, numbers);
        
        if (IsValidUpdateSequence(numbers, ordering)) {
            int mid = numbers.size() / 2;
            sum1 += numbers[mid];
        } else {
            FixSequence(numbers, ordering);
            int mid = numbers.size() / 2;
            sum2 += numbers[mid];
        }
    }

    std::cout << "    Part1: " << sum1 << std::endl;
    std::cout << "    Part2: " << sum2 << std::endl;
}
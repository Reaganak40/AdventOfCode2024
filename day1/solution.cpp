#include "pch.h"

void ReadInput(std::vector<int>& arr1, std::vector<int>& arr2)
{
    arr1.reserve(1000);
    arr2.reserve(1000);

    std::ifstream file("day1/input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // split line into two numbers
        std::string num1 = line.substr(0, line.find_first_of(' '));
        
        std::string num2 = line.substr(line.find_first_of(' ') + 1);
        num2.erase(std::remove_if(num2.begin(), num2.end(), [](unsigned char c) {
            return std::isspace(c);
        }), num2.end());

        arr1.push_back(std::stoi(num1));
        arr2.push_back(std::stoi(num2));
    }
}

void DoDay1()
{
    std::cout << "Day 1" << std::endl;
    int sum = 0;

    // read input
    std::vector<int> arr1;
    std::vector<int> arr2;
    std::unordered_map<int, int> occurances;
    ReadInput(arr1, arr2);

    // calculate difference scores
    for (int num : arr2) {
        if (occurances.find(num) == occurances.end()) {
            occurances[num] = 0;
        }
        occurances[num]++;  // count the number of times a number occurs
    }

    int similarity_score = 0;
    for (int num : arr1) {
        if (occurances.find(num) != occurances.end()) {
            similarity_score += num * occurances[num];
        }
    }

    // sort the arrays
    std::sort(arr1.begin(), arr1.end());
    std::sort(arr2.begin(), arr2.end());

    // sum the differences
    for (int i = 0; i < arr1.size(); i++)
    {
        sum += std::abs(arr2[i] - arr1[i]);
    }

    std::cout << "    Sum of Differences: " << sum << std::endl;
    std::cout << "    Similarity Score:   " << similarity_score << std::endl;
}
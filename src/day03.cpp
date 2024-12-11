#include "pch.h"

/**
 * @brief Finds all mul(x,x), multiplies them, and returns the sum\
 * @param input The input string
 * @return The sum of all mul(x,x) in the input string
 */
unsigned long long ParseInput(const std::string& input)
{
    unsigned long long sum = 0;
    for (int i = 0; i < input.size() - 7; i++) {

        // check for 'mul('
        if (input[i] == 'm' &&
            input[i + 1] == 'u' &&
            input[i + 2] == 'l' &&
            input[i + 3] == '(') {

            std::string num1 = "";
            std::string num2 = "";

            // get first number
            i += 4;
            for(; i < input.size(); i++) {
                if (std::isdigit(input[i])) {
                    num1 += input[i];
                } else {
                    break;
                }
            }
            if (input[i] != ',' || num1.empty()) {
                continue;
            }

            // get second number
            i++;
            for(; i < input.size(); i++) {
                if (std::isdigit(input[i])) {
                    num2 += input[i];
                } else {
                    break;
                }
            }
            if (input[i] != ')' || num2.empty()) {
                continue;
            }

            sum += std::stoi(num1) * std::stoi(num2);
        }
    }

    return sum;
}

/**
 * @brief Finds all mul(x,x), multiplies them, and returns the sum. Allows for do() and don't() to enable or disable the mul() function
 * @param input The input string
 * @return The sum of all mul(x,x) in the input string
 */
unsigned long long ParseInput2(const std::string& input)
{
    unsigned long long sum = 0;
    bool enabled = true;
    for (int i = 0; i < input.size() - 7; i++) {

        // check for 'do()'
        std::string sub = input.substr(i, 4);
        if (sub == "do()") {
            enabled = true;
            i += 3;
            continue;
        }

        // check for 'don't()'
        sub = input.substr(i, 7);
        if (sub == "don't()") {
            enabled = false;
            i += 6;
            continue;
        }

        if (!enabled) {
            continue;
        }

        // check for 'mul('
        if (input[i] == 'm' &&
            input[i + 1] == 'u' &&
            input[i + 2] == 'l' &&
            input[i + 3] == '(') {

            std::string num1 = "";
            std::string num2 = "";

            // get first number
            i += 4;
            for(; i < input.size(); i++) {
                if (std::isdigit(input[i])) {
                    num1 += input[i];
                } else {
                    break;
                }
            }
            if (input[i] != ',' || num1.empty()) {
                continue;
            }

            // get second number
            i++;
            for(; i < input.size(); i++) {
                if (std::isdigit(input[i])) {
                    num2 += input[i];
                } else {
                    break;
                }
            }
            if (input[i] != ')' || num2.empty()) {
                continue;
            }

            //std::cout << "mul(" << num1 << "," << num2 << ")" << std::endl;
            sum += std::stoi(num1) * std::stoi(num2);
        }
    }

    return sum;
}

void DoDay3()
{
    std::cout << "Day 3" << std::endl;
    std::ifstream file("inputs/day3/input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string input;
    std::string line;
    while (std::getline(file, line))
    {
        input += line;
    }

    std::cout << "    Part 1: " << ParseInput(input) << std::endl;
    std::cout << "    Part 2: " << ParseInput2(input) << std::endl;
}
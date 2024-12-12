#include "pch.h"
#include "utils.hpp"

struct TupleHash {
    std::size_t operator()(const std::tuple<uint64_t, int>& t) const {
        // Combine the hash values of the two integers
        return std::hash<uint64_t>()(std::get<0>(t)) ^ (std::hash<int>()(std::get<1>(t)) << 1);
    }
};

bool SplitInHalf(uint64_t number, uint64_t& out1, uint64_t& out2) {
    std::string num_str = std::to_string(number);
    if (num_str.size() % 2 != 0) {
        return false;
    }

    out1 = std::stoull(num_str.substr(0, num_str.size() / 2));
    out2 = std::stoull(num_str.substr(num_str.size() / 2));
    return true;
}

uint64_t Blink(const std::vector<uint64_t>& start, int part = 1)
{
    const int iterations = part == 1 ? 25 : 75;
    std::unordered_map<std::tuple<uint64_t, int>, uint64_t, TupleHash> cache;

    std::function<uint64_t(uint64_t, int)> do_blink = [&](uint64_t stone, int curr_i) -> uint64_t {
        if (curr_i == iterations) {
            return 1;
        }
        
        if (cache.find({ stone, curr_i }) != cache.end()) {
            return cache.at({ stone, curr_i });
        }

        if (stone == 0) {
            uint64_t sum = do_blink(1, curr_i + 1);
            cache[{stone, curr_i}] = sum;
            return sum;
        }

        // split into two numbers if even digits
        uint64_t left = 0;
        uint64_t right = 0;
        if (SplitInHalf(stone, left, right)) {
            uint64_t sum = do_blink(left, curr_i + 1) + do_blink(right, curr_i + 1);
            cache[{stone, curr_i}] = sum;
            return sum;
        }

        // multiply by 2024 if odd digits
        if (stone > 1.8014399e+16) {
            std::cout << "Possible overflow: " << stone << std::endl;
        }

        uint64_t sum = do_blink(stone * 2024, curr_i + 1);
        cache[{stone, curr_i}] = sum;
        return sum;
        };

    uint64_t sum = 0;
    for (auto stone : start) {
        sum += do_blink(stone, 0);
    }

    return sum;
}

void DoDay11()
{
    std::cout << "Day 11: " << std::endl;
    std::ifstream file("inputs/day11/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<uint64_t> stones;
    while (std::getline(file, line)) {
        SplitIntoLargeNumbers(line, stones);
    }

    std::cout << "    Part 1: " << Blink(stones, 1) << std::endl;
    std::cout << "    Part 2: " << Blink(stones, 2) << std::endl;

}



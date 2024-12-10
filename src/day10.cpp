#include "pch.h"
#include "utils.hpp"

// Hash function for std::tuple<int, int>
struct TupleHash {
    std::size_t operator()(const std::tuple<int, int>& t) const {
        // Combine the hash values of the two integers
        return std::hash<int>()(std::get<0>(t)) ^ (std::hash<int>()(std::get<1>(t)) << 1);
    }
};

int SumOfTrailheads(const std::vector<std::string>& grid, int part = 1)
{
    int sum = 0;

    // used to track endpoints already visited when traveling down a trail
    std::unordered_set<std::tuple<int, int>, TupleHash> endpoints;

    // Depth-first search to find completed trails
    std::function<int(int, int, int)> travel_trailhead = [&](int x, int y, int target) -> int {
        int curr = grid[y][x] - '0';
        if (target != curr) {
            return 0;
        }
        if (target == 9) {
            if (part == 2) {
                return 1;
            }

            if (endpoints.find({ x, y }) == endpoints.end()) {
                endpoints.emplace(x, y);
                return 1;
            }
            return 0;
        }

        int curr_sum = 0;
        if (x > 0) {
            curr_sum += travel_trailhead(x - 1, y, target + 1);
        }
        if (x < grid[0].size() - 1) {
            curr_sum += travel_trailhead(x + 1, y, target + 1);
        }
        if (y > 0) {
            curr_sum += travel_trailhead(x, y - 1, target + 1);
        }
        if (y < grid.size() - 1) {
            curr_sum += travel_trailhead(x, y + 1, target + 1);
        }
        return curr_sum;
    };

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] != '0') {
                continue;
            }

            sum += travel_trailhead(x, y, 0);
            endpoints.clear();
        }
    }

    return sum;
}

void DoDay10()
{
    std::cout << "Day 10: " << std::endl;
    std::ifstream file("inputs/day10/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> grid;
    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    std::cout << "    Part 1: " << SumOfTrailheads(grid, 1) << std::endl;
    std::cout << "    Part 2: " << SumOfTrailheads(grid, 2) << std::endl;
}
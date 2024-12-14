#include "pch.h"

// Hash function for std::tuple<int, int>
struct TupleHash {
    std::size_t operator()(const std::tuple<int, int>& t) const {
        // Combine the hash values of the two integers
        return std::hash<int>()(std::get<0>(t)) ^ (std::hash<int>()(std::get<1>(t)) << 1);
    }
};

/**
 * @brief Counts the unique locations for antinodes
 * @param grid The map of antennas with different frequencies
 * @paeam part Which problem part to solve
 * @return The number of unique locations, on the grid, that have an antinode
 */
int CountUniqueLocations(const std::vector<std::string>& grid, int part = 1) {
    std::unordered_set<std::tuple<int, int>, TupleHash> locations;

    auto in_bounds = [&](int x, int y) {
        return x >= 0 && x < grid[0].size() && y >= 0 && y < grid.size();
    };

    auto searchForAntinodes1 = [&](int x, int y) {
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (y == i && x == j) {
                    continue;
                }

                if (grid[i][j] == grid[y][x]) {
                    int dx = j - x;
                    int dy = i - y;

                    int x2 = j + dx;
                    int y2 = i + dy;
                    if (in_bounds(x2, y2)) {
                        locations.emplace(x2, y2);
                    }
                }
            }
        }
    };

    auto searchForAntinodes2 = [&](int x, int y) {
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (y == i && x == j) {
                    continue;
                }

                if (grid[i][j] == grid[y][x]) {
                    int dx = j - x;
                    int dy = i - y;

                    locations.emplace(x, y);
                    locations.emplace(j, i);

                    int x2 = j + dx;
                    int y2 = i + dy;
                    while (in_bounds(x2, y2)) {
                        locations.emplace(x2, y2);
                        x2 += dx;
                        y2 += dy;
                    }
                }
            }
        }
    };


    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] != '.') {
                if (part == 1) {
                    searchForAntinodes1(j, i);
                } else {
                    searchForAntinodes2(j, i);
                }
            }
        }
    }

    return locations.size();
}

void DoDay8() {
    std::cout << "Day 8" << std::endl;
    std::ifstream file("inputs/day08/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> grid;
    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    std::cout << "    Part 1: " << CountUniqueLocations(grid, 1) << std::endl;
    std::cout << "    Part 2: " << CountUniqueLocations(grid, 2) << std::endl;
}
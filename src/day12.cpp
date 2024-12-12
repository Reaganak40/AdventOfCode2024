#include "pch.h"
#include "utils.hpp"

uint64_t CalculatePrice(const std::vector<std::string>& grid)
{
    uint64_t price = 0;
    std::vector<std::vector<int>> visited(grid.size(), std::vector<int>(grid[0].size(), 0));

    auto in_bounds = [&](int x, int y) {
        return x >= 0 && x < grid[0].size() && y >= 0 && y < grid.size();
    };

    std::function<void(char, uint64_t, uint64_t, uint64_t&, uint64_t&)> dfs = [&](
        char plot, uint64_t x, uint64_t y, uint64_t& out_area, uint64_t& out_perimeter) {
        
        if (visited[y][x] == 1) {
            return;
        }
        visited[y][x] = 1;

        // assumes this was already checked
        out_area += 1;
        out_perimeter += 4;
        
        if (in_bounds(x - 1, y) && grid[y][x - 1] == plot) {
            out_perimeter -= 1;
            dfs(plot, x - 1, y, out_area, out_perimeter);
        }

        if (in_bounds(x + 1, y) && grid[y][x + 1] == plot) {
            out_perimeter -= 1;
            dfs(plot, x + 1, y, out_area, out_perimeter);
        }

        if (in_bounds(x, y - 1) && grid[y - 1][x] == plot) {
            out_perimeter -= 1;
            dfs(plot, x, y - 1, out_area, out_perimeter);
        }

        if (in_bounds(x, y + 1) && grid[y + 1][x] == plot) {
            out_perimeter -= 1;
            dfs(plot, x, y + 1, out_area, out_perimeter);
        }
    };

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (visited[y][x] == 1) {
                continue;
            }

            uint64_t area = 0;
            uint64_t perimeter = 0;
            dfs(grid[y][x], x, y, area, perimeter);
            price += (area * perimeter);
        }
    }

    return price;
}

void DoDay12()
{
    std::cout << "Day 12: " << std::endl;
    std::ifstream file("inputs/day12/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> grid;
    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    std::cout << "    Price: " << CalculatePrice(grid) << std::endl;
}



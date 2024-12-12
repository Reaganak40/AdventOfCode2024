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

uint64_t CalculatePrice2(const std::vector<std::string>& grid)
{
    uint64_t price = 0;

    std::vector<std::vector<int>> visited(grid.size(), std::vector<int>(grid[0].size(), 0));
    std::unordered_map<std::tuple<int, int>, std::vector<int>, TupleHashInt2> horizontal_edges;
    std::unordered_map<std::tuple<int, int>, std::vector<int>, TupleHashInt2> vertical_edges;

    auto in_bounds = [&](int x, int y) {
        return x >= 0 && x < grid[0].size() && y >= 0 && y < grid.size();
    };

    std::function<uint64_t(char, int, int)> dfs = [&](char plot, int x, int y) -> uint64_t {
        if (visited[y][x] == 1) {
            return 0;
        }

        uint64_t area = 1;
        visited[y][x] = 1;

        if (in_bounds(x - 1, y) && grid[y][x - 1] == plot) {
            area += dfs(plot, x - 1, y);
        }
        else {
            horizontal_edges[{x, x - 1}].push_back(y);
        }

        if (in_bounds(x + 1, y) && grid[y][x + 1] == plot) {
            area += dfs(plot, x + 1, y);
        }
        else {
            horizontal_edges[{x, x + 1}].push_back(y);
        }

        if (in_bounds(x, y - 1) && grid[y - 1][x] == plot) {
            area += dfs(plot, x, y - 1);
        }
        else {
            vertical_edges[{y, y - 1}].push_back(x);
        }

        if (in_bounds(x, y + 1) && grid[y + 1][x] == plot) {
            area += dfs(plot, x, y + 1);
        }
        else {
            vertical_edges[{y, y + 1}].push_back(x);
        }

        return area;
    };

    auto calculate_perimter = [&]() {
        uint64_t perimeter = 0;

        for (auto& [from, edge_list] : horizontal_edges) {
            if (edge_list.size() == 0) {
                continue;
            }

            std::sort(edge_list.begin(), edge_list.end());
            perimeter++;

            // chech for strictly increasing
            for (int i = 1; i < edge_list.size(); i++) {
                if (edge_list[i] - edge_list[i - 1] > 1) {
                    perimeter++;
                }
            }

            edge_list.clear();
        }

        for (auto& [from, edge_list] : vertical_edges) {
            if (edge_list.size() == 0) {
                continue;
            }

            std::sort(edge_list.begin(), edge_list.end());
            perimeter++;

            // chech for strictly increasing
            for (int i = 1; i < edge_list.size(); i++) {
                if (edge_list[i] - edge_list[i - 1] > 1) {
                    perimeter++;
                }
            }

            edge_list.clear();
        }

        return perimeter;
    };

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (visited[y][x] == 1) {
                continue;
            }

            uint64_t area = dfs(grid[y][x], x, y);
            uint64_t perimeter = calculate_perimter();
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

    std::cout << "    Part 1: " << CalculatePrice(grid) << std::endl;
    std::cout << "    Part 2: " << CalculatePrice2(grid) << std::endl;
}



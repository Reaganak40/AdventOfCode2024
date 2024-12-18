#include "pch.h"

#ifdef TEST
    #define GRID_WIDTH 7
    #define GRID_HEIGHT 7
    #define BYTES_DROPPED 12
    #define FILE_PATH "inputs/day18/input2.txt"
#else
    #define GRID_WIDTH 71
    #define GRID_HEIGHT 71
    #define BYTES_DROPPED 1024
    #define FILE_PATH "inputs/day18/input.txt"
#endif

template<typename T>
using Grid = T[GRID_WIDTH][GRID_HEIGHT];

struct Compare
{
    bool operator()(const std::pair<int, std::pair<int, int>>& a, const std::pair<int, std::pair<int, int>>& b)
    {
        return a.first > b.first;
    }
};

int ShortestPath(const Grid<char>& ram, std::pair<int, int> start, std::pair<int, int> end)
{
    // directions
    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}
    };

    // djiikstra
    Grid<int> dist;
    Grid<int> visited;
    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, Compare> pq;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            dist[y][x] = INT_MAX;
            visited[y][x] = false;
        }
    }

    dist[start.second][start.first] = 0;
    pq.push({0, start});

    while(!pq.empty())
    {
        auto [curr_dist, curr] = pq.top();
        pq.pop();

        if (visited[curr.second][curr.first]) continue;
        visited[curr.second][curr.first] = true;

        if (curr == end) {
            return curr_dist;
        }

        // check neighbors
        for (auto& dir : directions) {
            int x = curr.first + dir.first;
            int y = curr.second + dir.second;

            if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
                continue;
            }

            if (ram[y][x] == '#') {
                continue;
            }

            int new_dist = curr_dist + 1;
            if (new_dist < dist[y][x]) {
                dist[y][x] = new_dist;
                pq.push({new_dist, {x, y}});
            }
        }
    }

    return -1;
}

std::string FindUnsolvableMaze(Grid<char>& grid, std::vector<std::pair<int, int>> bytes)
{
    int curr_drop = BYTES_DROPPED;
    for (; curr_drop < bytes.size(); curr_drop++) {
        grid[bytes[curr_drop].second][bytes[curr_drop].first] = '#';
        if (ShortestPath(grid, {0, 0}, {GRID_WIDTH - 1, GRID_HEIGHT - 1}) == -1) {
            break;
        }
    }

    if (curr_drop == bytes.size()) {
        return "No unsolvable maze found";
    }

    return std::to_string(bytes[curr_drop].first) + "," + std::to_string(bytes[curr_drop].second);
}

void DoDay18()
{
    std::cout << "Day 18" << std::endl;

    std::ifstream file(FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    // init ram
    Grid<char> ram;
    std::vector<std::pair<int, int>> bytes;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            ram[y][x] = '.';
        }
    }

    // get byte drops
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        int x, y;
        sscanf(line.c_str(), "%d,%d", &x, &y);
        bytes.push_back({x, y});
    }

    // drop bytes into ram
    for (int i = 0; i < bytes.size(); i++) {
        if (i == BYTES_DROPPED) {
            break;
        }
        ram[bytes[i].second][bytes[i].first] = '#';
    }

    std::cout << "    Part 1: " << ShortestPath(ram, {0, 0}, {GRID_WIDTH - 1, GRID_HEIGHT - 1}) << std::endl;
    std::cout << "    Part 2: " << FindUnsolvableMaze(ram, bytes) << std::endl;
}
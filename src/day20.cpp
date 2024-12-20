#include "pch.h"
#include "utils.hpp"

#define FILE_PATH "inputs/day20/input.txt"

uint64_t SolveMaze(std::vector<std::string>& maze, int part)
{
    const int radius = part == 1 ? 2 : 20;
    const int time_save = 100;

    // init distance grid
    std::vector<std::vector<int>> dist;
    int start_x = 0, start_y = 0;
    for (int y = 0; y < maze.size(); y++) {
        dist.push_back(std::vector<int>(maze[y].size(), -1));
        for (int x = 0; x < maze[y].length(); x++) {
            if (maze[y][x] == 'S') {
                start_x = x;
                start_y = y;
            }
        }
    }

    // bfs
    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}
    };
    std::queue<std::pair<int, int>> q;
    q.push({start_x, start_y});
    dist[start_y][start_x] = 0;
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        if (maze[y][x] == 'E') {
            break;
        }

        for (auto& [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0 || nx >= maze[0].length() || ny < 0 || ny >= maze.size()) continue;
            if (maze[ny][nx] == '#') continue;
            if (dist[ny][nx] != -1) continue;

            dist[ny][nx] = dist[y][x] + 1;
            q.push({nx, ny});
        }
    }

    // find time saves
    uint64_t time_saves = 0;
    for (int y = 0; y < maze.size(); y++) {           // each row
        for (int x = 0; x < maze[y].length(); x++) {  // each column
            if (dist[y][x] == -1) continue;
            for (int r = 2; r <= radius; r++) {       // radius from start of cheat
                for (int dy = 0; dy <= r; dy++) {
                    int dx = r - dy;
                    std::unordered_set<std::pair<int, int>, TupleHashInt2> directions = {
                        {x + dx, y + dy}, // note: need to make set to avoid duplicates
                        {x - dx, y + dy},
                        {x + dx, y - dy},
                        {x - dx, y - dy}
                    };
                    for (auto& [nx, ny] : directions) {   // each direction (manhattan distance)
                        if (nx < 0 || nx >= maze[0].length() || ny < 0 || ny >= maze.size()) continue;
                        if (maze[ny][nx] == '#') continue;
                        if ((dist[y][x] - dist[ny][nx]) >= (time_save + r)) time_saves++;
                    }
                }
            }
        }
    }

    return time_saves;
}

void DoDay20()
{
    std::cout << "Day 20" << std::endl;

    std::ifstream file(FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    // get input
    std::string line;
    std::vector<std::string> maze;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        maze.push_back(line);
    }

    std::cout << "    Part 1: " << SolveMaze(maze, 1) << std::endl;
    std::cout << "    Part 2: " << SolveMaze(maze, 2) << std::endl;
}
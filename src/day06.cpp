#include "pch.h"

#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3

/**
 * @brief Get the number of unique steps taken by a robot in a grid
 * @param grid The grid to traverse
 * @param loop_detected Whether a loop was detected
 * @return The number of unique steps taken by the robot
 */
int GetUniqueSteps(std::vector<std::string>& grid, bool* loop_detected = nullptr)
{
    std::vector<std::vector<std::array<int, 4>>> visited;
    char guard = ' ';
    for (int i = 0; i < grid.size(); i++)
    {
        visited.push_back(std::vector<std::array<int, 4>>());
        for (int j = 0; j < grid[i].size(); j++)
        {
            visited[i].push_back({0, 0, 0, 0});
        }
    }

    auto detect_loop = [&visited, &grid](int x, int y, int dx, int dy) {
        if (grid[y][x] != 'X')
        {
            return false;
        }

        int direction = 0;
        if (dx == 0 && dy == -1) {
            direction = UP;
        } else if (dx == 1 && dy == 0) {
            direction = RIGHT;
        } else if (dx == 0 && dy == 1) {
            direction = DOWN;
        } else if (dx == -1 && dy == 0) {
            direction = LEFT;
        }

        return visited[y][x][direction] > 0;
    };

    auto update_direction = [&visited](int x, int y, int dx, int dy) {
        int direction = 0;
        if (dx == 0 && dy == -1) {
            direction = UP;
        } else if (dx == 1 && dy == 0) {
            direction = RIGHT;
        } else if (dx == 0 && dy == 1) {
            direction = DOWN;
        } else if (dx == -1 && dy == 0) {
            direction = LEFT;
        }

        visited[y][x][direction]++;
    };

    // get starting position and direction
    int start_x = 0, start_y = 0;
    int x = 0, y = 0;
    int dx = 0, dy = 0;
    
    for (; y < grid.size(); y++)
    {
        for (x = 0; x < grid[y].size(); x++)
        {
            guard = grid[y][x];
            if (grid[y][x] == '^')
            {
                dy = -1;
                goto simulate_steps;
            }

            if (grid[y][x] == 'v')
            {
                dy = 1;
                goto simulate_steps;
            }

            if (grid[y][x] == '<')
            {
                dx = -1;
                goto simulate_steps;
            }

            if (grid[y][x] == '>')
            {
                dx = 1;
                goto simulate_steps;
            }
        }
    }

    std::cout << "Could not find starting position" << std::endl;
    return 0;

    simulate_steps:
    start_x = x;
    start_y = y;

    auto in_bounds = [&grid](int x, int y) {
        return x >= 0 && x < grid[0].size() && y >= 0 && y < grid.size();
    };

    int steps = 0;
    while(in_bounds(x, y))
    {
        if (detect_loop(x, y, dx, dy))
        {
            if (loop_detected != nullptr) *loop_detected = true;
            break;
        }

        int n_x = x + dx;
        int n_y = y + dy;

        if (in_bounds(n_x, n_y)) {
            if (grid[n_y][n_x] == '#')
            {
                if (dx == 0 && dy == -1) {
                    dx = 1;
                    dy = 0;
                } else if (dx == 1 && dy == 0) {
                    dx = 0;
                    dy = 1;
                } else if (dx == 0 && dy == 1) {
                    dx = -1;
                    dy = 0;
                } else if (dx == -1 && dy == 0) {
                    dx = 0;
                    dy = -1;
                }

                continue;
            }
        }

        if (grid[y][x] != 'X')
        {
            grid[y][x] = 'X';
            steps++;
        }
        update_direction(x, y, dx, dy);

        x += dx;
        y += dy;
    }

    // clean up grid
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == 'X')
            {
                grid[i][j] = '.';
            }
        }
    }
    grid[start_y][start_x] = guard;

    return steps;
}

int GetPossibleLoops(std::vector<std::string>& grid)
{
    int possible_loops = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '.')
            {
                grid[i][j] = '#';
                bool loop_detected = false;
                (void)GetUniqueSteps(grid, &loop_detected);
                if (loop_detected)
                {
                    possible_loops++;
                }

                grid[i][j] = '.';
            }
        }
    }

    return possible_loops;
}

void DoDay6()
{
    std::cout << "Day 6" << std::endl;
    std::ifstream file("inputs/day06/input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> grid;
    while (std::getline(file, line))
    {
        grid.push_back(line);
    }

    std::cout << "    Unique steps: " << GetUniqueSteps(grid) << std::endl;
    std::cout << "    Possible loops: " << GetPossibleLoops(grid) << std::endl;
}
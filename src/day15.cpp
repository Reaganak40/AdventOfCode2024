#include "pch.h"

uint64_t MoveRobot(std::vector<std::string>& grid, const std::string& move_sequence)
{
    // locate the robot
    int robot_x = 0;
    int robot_y = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == '@') {
                robot_x = x;
                robot_y = y;
                break;
            }
        }
    }

    std::function<bool(int, int, int, int)> move = [&](int x, int y, int dx, int dy) -> bool {
        int new_x = x + dx;
        int new_y = y + dy;
        if (new_x < 0 || new_x >= grid[0].size() || new_y < 0 || new_y >= grid.size()) {
            return false;
        }

        if (grid[new_y][new_x] == '#') {
            return false;
        }

        if(grid[new_y][new_x] == '.') {
            grid[new_y][new_x] = grid[y][x];
            grid[y][x] = '.';
            return true;
        }

        if (grid[new_y][new_x] == 'O') {
            if (move(new_x, new_y, dx, dy)) {
                grid[new_y][new_x] = grid[y][x];
                grid[y][x] = '.';
                return true;
            }
        }

        return false;
    };

    // move the robot per the sequence
    for (char dir : move_sequence) {
        int dx = 0;
        int dy = 0;
        switch (dir) {
            case '<':
                dx = -1;
                break;
            case '>':
                dx = 1;
                break;
            case '^':
                dy = -1;
                break;
            case 'v':
                dy = 1;
                break;
            default:
                std::cout << "Invalid direction: " << dir << std::endl;
                continue;
        }

        if (move(robot_x, robot_y, dx, dy)) {
            robot_x += dx;
            robot_y += dy;
        }
    }

    uint64_t sum_of_gps = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == 'O') {
                sum_of_gps += (100 * y) + x;
            }
        }
    }

    return sum_of_gps;
}

uint64_t MoveRobot2(std::vector<std::string>& grid, const std::string& move_sequence)
{
    // locate the robot
    int robot_x = 0;
    int robot_y = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == '@') {
                robot_x = x;
                robot_y = y;
                break;
            }
        }
    }

    // when test is true - does not move anything, just checks if it can move
    std::function<bool(int, int, int, int, bool)> move = [&](int x, int y, int dx, int dy, bool test) -> bool {
        int new_x = x + dx;
        int new_y = y + dy;
        if (new_x < 0 || new_x >= grid[0].size() || new_y < 0 || new_y >= grid.size()) {
            return false;
        }

        if (grid[new_y][new_x] == '#') {
            return false;
        }

        if(grid[new_y][new_x] == '.') {
            if (!test) {
                grid[new_y][new_x] = grid[y][x];
                grid[y][x] = '.';
            }
            return true;
        }

        // moving left, right wont cause cause two boxes to be pushed at the same time
        if (dy == 0) {
            if (grid[new_y][new_x] == '[' || grid[new_y][new_x] == ']') {
                if (move(new_x, new_y, dx, dy, test)) {
                    if (!test) {
                        grid[new_y][new_x] = grid[y][x];
                        grid[y][x] = '.';
                    }
                    return true;
                }
            }
        } else {
            // moving up, down requires checking both sides of the box
            if (grid[new_y][new_x] == '[') {
                bool can_move_left = move(new_x, new_y, dx, dy, true);
                bool can_move_right = move(new_x + 1, new_y, dx, dy, true);

                if (can_move_left && can_move_right) {
                    move(new_x, new_y, dx, dy, test);
                    move(new_x + 1, new_y, dx, dy, test);
                    if(!test) {
                        grid[new_y][new_x] = grid[y][x];
                        grid[y][x] = '.';
                    }
                    return true;
                }
            } else if (grid[new_y][new_x] == ']') {
                bool can_move_left = move(new_x - 1, new_y, dx, dy, true);
                bool can_move_right = move(new_x, new_y, dx, dy, true);

                if (can_move_left && can_move_right) {
                    move(new_x - 1, new_y, dx, dy, test);
                    move(new_x, new_y, dx, dy, test);
                    if(!test) {
                        grid[new_y][new_x] = grid[y][x];
                        grid[y][x] = '.';
                    }
                    return true;
                }
            }
        }

        return false;
    };

    // move the robot per the sequence
    for (char dir : move_sequence) {
        int dx = 0;
        int dy = 0;
        switch (dir) {
            case '<':
                dx = -1;
                break;
            case '>':
                dx = 1;
                break;
            case '^':
                dy = -1;
                break;
            case 'v':
                dy = 1;
                break;
            default:
                std::cout << "Invalid direction: " << dir << std::endl;
                continue;
        }

        if (move(robot_x, robot_y, dx, dy, false)) {
            robot_x += dx;
            robot_y += dy;
        }
    }

    uint64_t sum_of_gps = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == '[') {
                sum_of_gps += (100 * y) + x;
            }
        }
    }

    return sum_of_gps;
}

void DoDay15()
{
     std::cout << "Day 15: " << std::endl;
    std::ifstream file("inputs/day15/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> grid1;
    std::vector<std::string> grid2;
    std::string move_sequence;

    // get input
    bool make_grid = true;
    while (std::getline(file, line)) {
        if (line.empty()) {
            make_grid = false;
            continue;
        }
        if (make_grid) {
            grid1.push_back(line);
            grid2.push_back(line);
        } else {
            move_sequence += line;
        }
    }

    // create grid2
    for (auto& line : grid2) {
        for (int i = 0; i < line.size(); i++) {
            switch (line[i]) {
                case '#':
                    line.insert(i, "#");
                    i++;
                    break;
                case 'O':
                    line[i] = '[';
                    line.insert(i + 1, "]");
                    break;
                case '.':
                    line.insert(i, ".");
                    i++;
                    break;
                case '@':
                    line.insert(i + 1, ".");
                    i++;
                    break;
                default:
                    break;
            }
        }
    }

    std::cout << "    Part 1: " << MoveRobot(grid1, move_sequence) << std::endl;
    std::cout << "    Part 2: " << MoveRobot2(grid2, move_sequence) << std::endl;
}
#include "pch.h"
#include "utils.hpp"

// #define TILE_WIDTH 101
// #define TILE_HEIGHT 103

#define TILE_WIDTH 101
#define TILE_HEIGHT 103

#define LINE_THRESHOLD 7
#define MINIMUM_LINES_FOR_TREE 5

struct Robot
{
    // [x, y]
    int pos[2];
    int dir[2];
};

#define NOT_IN_QUADRANT -1


uint64_t SimulateRobots(const std::vector<Robot>& robots)
{
    std::array<std::array<int, TILE_WIDTH>, TILE_HEIGHT> grid;
    for (int y = 0; y < TILE_HEIGHT; y++) {
        for (int x = 0; x < TILE_WIDTH; x++) {
            grid[y][x] = 0;
        }
    }

    uint64_t quadrants[4] = { 0, 0, 0, 0 };
    auto determineQuadrant = [](int x, int y) -> int {
        if (x < TILE_WIDTH / 2) {
            if (y < TILE_HEIGHT / 2) {
                return 0;
            } else if (y >= Ceil(TILE_HEIGHT, 2)) {
                return 1;
            }
            return NOT_IN_QUADRANT;
        } else if (x >= Ceil(TILE_WIDTH, 2)) {
            if (y < TILE_HEIGHT / 2) {
                return 2;
            } else if (y >= Ceil(TILE_HEIGHT, 2)) {
                return 3;
            }
            return NOT_IN_QUADRANT;
        }

        return NOT_IN_QUADRANT;
    };

    for (const auto& robot : robots) {
        int x = robot.pos[0];
        int y = robot.pos[1];

        for (int i = 0; i < 100; i++) {
            x = (x + robot.dir[0] + TILE_WIDTH) % TILE_WIDTH;
            y = (y + robot.dir[1] + TILE_HEIGHT) % TILE_HEIGHT;
        }
        int quadrant = determineQuadrant(x, y);
        if (quadrant != NOT_IN_QUADRANT) {
            quadrants[quadrant]++;
        }
    }

    return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

uint64_t FindEasterEgg(std::vector<Robot>& robots, bool show_tree = false)
{
    std::array<std::array<int, TILE_WIDTH>, TILE_HEIGHT> grid;
    for (int y = 0; y < TILE_HEIGHT; y++) {
        for (int x = 0; x < TILE_WIDTH; x++) {
            grid[y][x] = 0;
        }
    }

    // init robot locations
    for (const auto& robot : robots) {
        grid[robot.pos[1]][robot.pos[0]]++;
    }

    auto could_be_christmas_tree = [&grid]() -> bool {
        // see how many straight lines we have greater than our threshold
        uint64_t count = 0;

        // check horizontal lines
        for (int y = 0; y < TILE_HEIGHT; y++) {
            uint64_t line_count = 0;
            for (int x = 0; x < TILE_WIDTH; x++) {
                if (grid[y][x] > 0) {
                    line_count++;
                } else {
                    if (line_count >= LINE_THRESHOLD) {
                        count++;
                    }
                    line_count = 0;
                }
            }
            if (line_count >= LINE_THRESHOLD) {
                count++;
            }
        }

        // check vertical lines
        for (int x = 0; x < TILE_WIDTH; x++) {
            uint64_t line_count = 0;
            for (int y = 0; y < TILE_HEIGHT; y++) {
                if (grid[y][x] > 0) {
                    line_count++;
                } else {
                    if (line_count >= LINE_THRESHOLD) {
                        count++;
                    }
                    line_count = 0;
                }
            }
            if (line_count >= LINE_THRESHOLD) {
                count++;
            }
        }

        return count >= MINIMUM_LINES_FOR_TREE;
    };

    uint64_t iterations = 1;
    while (1) {
        for (auto& robot : robots) {
            grid[robot.pos[1]][robot.pos[0]]--;
            robot.pos[0] = (robot.pos[0] + robot.dir[0] + TILE_WIDTH) % TILE_WIDTH;
            robot.pos[1] = (robot.pos[1] + robot.dir[1] + TILE_HEIGHT) % TILE_HEIGHT;
            grid[robot.pos[1]][robot.pos[0]]++;
        }

        if (could_be_christmas_tree()) {
            // print grid
            if (show_tree) {
                for (int y = 0; y < TILE_HEIGHT; y++) {
                    for (int x = 0; x < TILE_WIDTH; x++) {
                        if (grid[y][x] == 0) {
                            std::cout << " ";
                        } else {
                            std::cout << "X";
                        }
                    }
                    std::cout << std::endl;
                }
            }
            std::cout << "    Part 2: " << iterations << std::endl;
            break;
        }

        iterations++;
    }

    return 0;
}

void DoDay14()
{
     std::cout << "Day 14: " << std::endl;
    std::ifstream file("inputs/day14/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<Robot> robots;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        Robot robot;
        (void)sscanf(line.c_str(), "p=%d,%d v=%d,%d", &robot.pos[0], &robot.pos[1], &robot.dir[0], &robot.dir[1]);
        robots.push_back(robot);
    }

    std::cout << "    Part 1: " << SimulateRobots(robots) << std::endl;
    FindEasterEgg(robots, false);
}
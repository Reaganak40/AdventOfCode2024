#include "pch.h"
#include "utils.hpp"

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define INFINITY ((uint64_t)-1)

// each edge in the graph needs to store the x,y position and direction between nodes
using Node = std::pair<int, int>;
using Graph = std::unordered_map<Node, std::vector<std::pair<Node, uint64_t[4]>>, TupleHashInt2>;

void CalculateWeights(const Node& from, const Node& to, uint64_t weights[4])
{
    weights[DIR_UP] = 0;
    weights[DIR_DOWN] = 0;
    weights[DIR_LEFT] = 0;
    weights[DIR_RIGHT] = 0;

    // actual direction
    int dx = to.first - from.first;
    int dy = to.second - from.second;


}

uint64_t SolveMaze(const std::vector<std::string>& maze)
{
    
}

void DoDay16()
{
    std::cout << "Day 16" << std::endl;

    std::ifstream file("inputs/day15/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> maze;

    // get input
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        maze.push_back(line);
    }
}
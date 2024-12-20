#include "pch.h"

#define CHEAT_OFF 0
#define CHEAT_ON_1 1
#define CHEAT_ON_2 2
#define CHEAT_DISABLED 3

struct BFSNode
{
    int x, y;
    int distance;
};

struct DFSNode
{
    int x, y;
    int cheat;
    int direction;
};

int BFS(const std::vector<std::string>& maze)
{
    int start_x, start_y, end_x, end_y;

    std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0)
    };

    // find start and end
    for (int y = 0; y < maze.size(); y++) {
        for (int x = 0; x < maze[y].length(); x++) {
            if (maze[y][x] == 'S') {
                start_x = x;
                start_y = y;
            }
            else if (maze[y][x] == 'E') {
                end_x = x;
                end_y = y;
            }
        }
    }

    // solve using BFS
    std::queue<BFSNode> q;
    std::vector<std::vector<int>> visited(maze.size(), std::vector<int>(maze[0].length(), 0));
    q.push({ start_x, start_y, CHEAT_OFF });

    while(!q.empty()) {

        BFSNode node = q.front();
        q.pop();

        if (node.x == end_x && node.y == end_y) {
            return node.distance;
        }

        for (const auto& dir : directions) {
            int new_x = node.x + dir.first;
            int new_y = node.y + dir.second;

            if (new_x < 0 || new_x >= maze[0].length() || new_y < 0 || new_y >= maze.size()) {
                continue;
            }

            if (visited[new_y][new_x] == 1) {
                continue;
            }

            if (maze[new_y][new_x] == '#') {
                continue;
            }

            q.push({ new_x, new_y, node.distance + 1 });
            visited[new_y][new_x] = 1;
        }
    }

    return -1;
}

int DFS(const std::vector<std::string>& maze, int maximum_time)
{
    int start_x, start_y, end_x, end_y;

    std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0)
    };

    // find start and end
    for (int y = 0; y < maze.size(); y++) {
        for (int x = 0; x < maze[y].length(); x++) {
            if (maze[y][x] == 'S') {
                start_x = x;
                start_y = y;
            }
            else if (maze[y][x] == 'E') {
                end_x = x;
                end_y = y;
            }
        }
    }

    // solve using DFS
    std::vector<int> solves;
    std::vector<DFSNode> stack;
    std::vector<std::vector<int>> visited(maze.size(), std::vector<int>(maze[0].length(), 0));
    stack.push_back({ start_x, start_y, CHEAT_OFF, 0 });

    // check every possible path
    while(!stack.empty()) {

        DFSNode node = stack.back();
        stack.pop_back();

        // check if we have reached the end
        if (node.x == end_x && node.y == end_y) {
            solves.push_back(stack.size());
            continue;
        }

        // check if we have reached the maximum time
        if (node.x < 0 || node.x >= maze[0].length() || node.y < 0 || node.y >= maze.size()) {
            continue;
        }

        // check if we reached maximum depth
        if (solves.size() >= maximum_time) {
            continue;
        }

        // check if this node is out of options
        if (node.direction > 3) {
            continue;
        }

        if (visited[node.y][node.x] == 1) {
            continue;
        }

        const auto& dir = directions[node.direction++];
        int new_x = node.x + dir.first;
        int new_y = node.y + dir.second;


        for (const auto& dir : directions) {
            int new_x = node.x + dir.first;
            int new_y = node.y + dir.second;

            if (new_x < 0 || new_x >= maze[0].length() || new_y < 0 || new_y >= maze.size()) {
                continue;
            }

            if (visited[new_y][new_x] == 1) {
                continue;
            }

            if (maze[new_y][new_x] == '#') {
                continue;
            }

            int cheat = node.cheat;
            if (maze[new_y][new_x] == 'C') {
                if (cheat == CHEAT_OFF) {
                    cheat = CHEAT_ON_1;
                }
                else if (cheat == CHEAT_ON_1) {
                    cheat = CHEAT_ON_2;
                }
                else if (cheat == CHEAT_ON_2) {
                    cheat = CHEAT_DISABLED;
                }
            }

            stack.push_back({ new_x, new_y, cheat, node.direction + 1 });
            visited[new_y][new_x] = 1;
        }
    }

}

void DoDay20()
{
    std::cout << "Day 20" << std::endl;

    std::ifstream file("inputs/day20/input2.txt");
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

    std::cout << "    Part 1: " << BFS(maze) << std::endl;
}
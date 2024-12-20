#include "pch.h"

#define NOT_FOUND ((uint64_t)-1)

// how much time to save compared to not cheating
#define SAVE_GOAL 2

struct BFSNode
{
    int x, y;
    uint64_t distance;
};

struct DFSNode
{
    int x, y;
    bool used_cheat;
    uint64_t depth;

    bool operator==(const DFSNode& other) const
    {
        return x == other.x && y == other.y && used_cheat == other.used_cheat && depth == other.depth;
    }
};

struct DFSNodeHash
{
    std::size_t operator()(const DFSNode& node) const
    {
        return std::hash<int>()(node.x) ^ std::hash<int>()(node.y) ^ std::hash<bool>()(node.used_cheat) ^ std::hash<uint64_t>()(node.depth);
    }
};

// finds the shortest path without cheating
uint64_t BFS(const std::vector<std::string>& maze)
{
    int start_x = 0, start_y = 0, end_x = 0, end_y = 0;

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
    q.push({ start_x, start_y, 0 });

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

    return NOT_FOUND;
}

// gets number of paths that save time by cheating
uint64_t DFS(const std::vector<std::string>& maze, uint64_t maximum_time)
{
    int start_x = 0, start_y = 0, end_x = 0, end_y = 0;

    std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(1, 0),
        std::make_pair(0, 1),
        std::make_pair(0, -1),
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

    std::unordered_map<DFSNode, uint64_t, DFSNodeHash> memo;
    std::unordered_set<std::pair<int, int>> successful_cheats;

    std::function<uint64_t(DFSNode)> dfs_search = [&](DFSNode node) -> uint64_t {
        if (memo.find(node) != memo.end()) {
            return memo.at(node);
        }

        if (node.depth > maximum_time) {
            return 0;
        }

        if (node.x == end_x && node.y == end_y) {
            return 1;
        }


        int result = 0;
        for (const auto& dir : directions) {
            int new_x = node.x + dir.first;
            int new_y = node.y + dir.second;
            bool new_cheat = node.used_cheat;

            if (new_x < 0 || new_x >= maze[0].length() || new_y < 0 || new_y >= maze.size()) {
                continue;
            }

           
            if (maze[new_y][new_x] == '#') {
                if (node.used_cheat) {
                    continue;
                }
                new_cheat = true;
            }

            result += dfs_search({ new_x, new_y, new_cheat, node.depth + 1 });
        }

        memo[node] = result;
        return result;
    };

    uint64_t res = dfs_search({ start_x, start_y, false, 0 });
    return res;
}

uint64_t SolveMaze(const std::vector<std::string>& maze)
{
    uint64_t max_time = BFS(maze);
    if (max_time == NOT_FOUND) {
        return DFS(maze, max_time - 1);
    }
    else if (max_time < SAVE_GOAL) {
        return 0;
    }
    else {
        return DFS(maze, max_time - SAVE_GOAL);
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

    std::cout << "    Part 1: " << SolveMaze(maze) << std::endl;
}
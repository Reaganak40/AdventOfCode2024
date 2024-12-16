#include "pch.h"
#include "utils.hpp"

#define INF ((uint64_t)-1)
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum class Direction
{
    North,
    East,
    South,
    West,
    NotADir,
};

struct Node
{
    int x;
    int y;

    // where we came from
    Direction dir;

    bool operator==(const Node& other) const
    {
        return x == other.x && y == other.y && dir == other.dir;
    }
};


struct NodeHash
{
    std::size_t operator()(const Node& node) const
    {
        return std::hash<int>()(node.x) ^ std::hash<int>()(node.y) ^ std::hash<int>()(static_cast<int>(node.dir));
    }
};

using Graph = std::unordered_map<Node, std::vector<Node>, NodeHash>;

struct NodeCompare
{
    bool operator()(const std::pair<uint64_t, Node>& a, const std::pair<uint64_t, Node>& b)
    {
        return a.first > b.first;
    }
};

void InitGraph(const std::vector<std::string>& maze, Graph& graph, Node& start, Node& end)
{
    for (int y = 0; y < maze.size(); y++) {
        for (int x = 0; x < maze[y].size(); x++) {
            if (maze[y][x] == '#') {
                continue;
            }
            else if (maze[y][x] == 'S') {
                start = { x, y, Direction::East };
            }
            else if (maze[y][x] == 'E') {
                end = { x, y, Direction::NotADir };
            }

            for (Direction dir = Direction::North;
                dir != Direction::NotADir;
                dir = static_cast<Direction>(static_cast<int>(dir) + 1)) {
                Node node = { x, y, dir };

                // check if direction is valid
                if (node.x != start.x || node.y != start.y) {
                    switch (dir) {
                    case Direction::North:
                        if (y + 1 >= maze.size() || maze[y + 1][x] == '#') {
                            continue;
                        }
                        break;
                    case Direction::East:
                        if (x - 1 < 0 || maze[y][x - 1] == '#') {
                            continue;
                        }
                        break;
                    case Direction::South:
                        if (y - 1 < 0 || maze[y - 1][x] == '#') {
                            continue;
                        }
                        break;
                    case Direction::West:
                        if (x + 1 >= maze[y].size() || maze[y][x + 1] == '#') {
                            continue;
                        }
                        break;
                    }
                }
                graph[node] = {};
                
                // check up
                int n_x = x;
                int n_y = y - 1;
                if (n_y >= 0 && maze[n_y][n_x] != '#') {
                    graph.at(node).push_back({n_x, n_y, Direction::North});
                }

                // check down
                n_x = x;
                n_y = y + 1;
                if (n_y < maze.size() && maze[n_y][n_x] != '#') {
                    graph.at(node).push_back({ n_x, n_y, Direction::South });
                }

                // check left
                n_x = x - 1;
                n_y = y;
                if (n_x >= 0 && maze[n_y][n_x] != '#') {
                    graph.at(node).push_back({ n_x, n_y, Direction::West });
                }

                // check right
                n_x = x + 1;
                n_y = y;
                if (n_x < maze[y].size() && maze[n_y][n_x] != '#') {
                    graph.at(node).push_back({ n_x, n_y, Direction::East });
                }
            }
        }
    }
}

bool IsValid(const Node& node)
{
    return node.x >= 0 && node.y >= 0;
}

bool Is180(const Node& a, const Node& b)
{
    return 
        a.dir == Direction::North && b.dir == Direction::South ||
        a.dir == Direction::South && b.dir == Direction::North ||
        a.dir == Direction::East && b.dir == Direction::West ||
        a.dir == Direction::West && b.dir == Direction::East;
}

uint64_t SolveMaze(std::vector<std::string>& maze)
{
    Graph graph;
    Node start;
    Node end;
    InitGraph(maze, graph, start, end);
    if (graph.empty()) {
        std::cout << "Graph is empty" << std::endl;
        return 0;
    }
    if (!IsValid(start) || !IsValid(end)) {
        std::cout << "Invalid start or end node" << std::endl;
        return 0;
    }

    // dijkstra
    std::priority_queue<std::pair<uint64_t, Node>, std::vector<std::pair<uint64_t, Node>>, NodeCompare> pq;
    std::unordered_map<Node, uint64_t, NodeHash> dist;
    std::unordered_map<Node, Node, NodeHash> prev;

    // initialize dist, prev, and pq
    for (auto& [node, edges] : graph) {
        dist[node] = INF;
        prev[node] = { -1, -1, Direction::NotADir };
    }
    dist[start] = 0;
    pq.push({ 0, start });

    while (!pq.empty()) {
        auto [curr_dist, curr_node] = pq.top();
        pq.pop();
        if (curr_node.x == end.x && curr_node.y == end.y) {
            end = curr_node;
            break;
        }

        for (auto& next_node : graph[curr_node]) {
            if (Is180(prev[curr_node],next_node)) {
                continue;
            }
            uint64_t alt = dist[curr_node] + (next_node.dir == curr_node.dir ? 1 : 1001);
            if (alt < dist[next_node]) {
                dist[next_node] = alt;
                prev[next_node] = curr_node;
                pq.push({ alt, next_node });
            }
        }
    }

    // show on maze
    Node curr = end;
    while (curr != start) {
        maze[curr.y][curr.x] = '+';
        curr = prev[curr];
    }

    int moves = 0;
    for (auto& line : maze) {
        for (auto c : line) {
            if (c == '+') {
                moves++;
                std::cout << ANSI_COLOR_RED << c << ANSI_COLOR_RESET;
            }
            else {
                std::cout << c;
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Distance: " << dist[end] << std::endl;
    std::cout << "Moves: " << moves << std::endl;
    return dist[end];
}

void DoDay16()
{
    std::cout << "Day 16" << std::endl;

    std::ifstream file("inputs/day16/input.txt");
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

    SolveMaze(maze);
    //std::cout << "    Part 1: " << SolveMaze(maze) << std::endl;
}
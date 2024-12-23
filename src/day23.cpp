#include "pch.h"

struct Edge
{
    char from[2];
    char to[2];
};

using Graph = std::unordered_map<std::string, std::vector<std::string>>;
void CreateGraph(const std::vector<Edge>& connections, Graph& graph)
{
    for (const auto& [from_c, to_c] : connections) {
        std::string from = {from_c[0], from_c[1]};
        std::string to = {to_c[0], to_c[1]};
        graph[from].push_back(to);
        graph[to].push_back(from);
    }
}

int FindGroups(const Graph& graph, int group_size)
{
    std::unordered_set<std::string> groups;

    auto add_group = [&](const std::vector<const std::string*>& group) {
        // create sorted copy
        std::vector<const std::string*> sorted_group = group;
        std::sort(sorted_group.begin(), sorted_group.end(), [](const std::string* a, const std::string* b) {
            return *a < *b;
        });

        std::string group_str;
        for (const auto& node : sorted_group) {
            group_str += *node;
        }
        groups.insert(group_str);
    };

    std::function<void(std::vector<const std::string*>&)> dfs = [&](std::vector<const std::string*>& curr_route)
    {
        if (curr_route.size() == group_size) {
            // check if last node has a connection to the first node
            if (std::find(
                graph.at(*curr_route.back()).begin(),
                graph.at(*curr_route.back()).end(), *curr_route.front()) != graph.at(*curr_route.back()).end()) {
                add_group(curr_route);
            }
            return;
        }

        for (const auto& connection : graph.at(*curr_route.back())) {

            // check if already in route
            if (std::find_if(curr_route.begin(), curr_route.end(),
            [&](const std::string* n) { return *n == connection; }) != curr_route.end()) {
                continue;
            }

            curr_route.push_back(&connection);
            dfs(curr_route);
            curr_route.pop_back();
        }
    };

    for (const auto& [node, connections] : graph) {
        std::vector<const std::string*> route = {&node};
        dfs(route);
    }

    // return number of groups that contain a node that start with 't'
    return std::count_if(groups.begin(), groups.end(), [](const std::string& group) {
        return group[0] == 't' || group[2] == 't' || group[4] == 't';
    });
}

std::string FindLargestGroup(const Graph& graph)
{
    std::string largest_group;
    int largest_group_size = 0;
    std::unordered_set<std::string> memo;

    // also sets largest_group_size
    auto in_memo = [&](const std::vector<const std::string*>& group) {
        std::vector<const std::string*> sorted_group = group;
        std::sort(sorted_group.begin(), sorted_group.end(), [](const std::string* a, const std::string* b) {
            return *a < *b;
        });
        
        std::string group_str;
        for (int i = 0; i < sorted_group.size(); i++) {
            group_str += *sorted_group[i];
            if (i < sorted_group.size() - 1) {
                group_str += ",";
            }
        }

        if (memo.find(group_str) != memo.end()) {
            return true;
        }

        memo.insert(group_str);
        if (group.size() > largest_group_size) {
            largest_group_size = group.size();
            largest_group = group_str;
        }

        return false;
    };

    // checks if the next node is present in each node in the group
    auto is_group_node = [&](std::vector<const std::string*>& curr_route, const std::string& next) {
        for (const auto& node : curr_route) {
            if (std::find(graph.at(*node).begin(), graph.at(*node).end(), next) == graph.at(*node).end()) {
                return false;
            }
        }
        return true;
    };

    // dfs where each node in the group must be able to connect to every other node in the group
    std::function<void(std::vector<const std::string*>&)> dfs = [&](std::vector<const std::string*>& curr_route)
    {
        if (in_memo(curr_route)) {
            return;
        }

        for (const auto& connection : graph.at(*curr_route.back())) {

            // check if already in route
            if (std::find_if(curr_route.begin(), curr_route.end(),
            [&](const std::string* n) { return *n == connection; }) != curr_route.end()) {
                continue;
            }

            if (is_group_node(curr_route, connection)) {
                curr_route.push_back(&connection);
                dfs(curr_route);
                curr_route.pop_back();
            }
        }
    };

    for (const auto& [node, connections] : graph) {
        std::vector<const std::string*> route = {&node};
        dfs(route);
    }

    return largest_group;
}

void DoDay23()
{
    std::cout << "Day 23" << std::endl;

    std::ifstream file("inputs/day23/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    // get input
    std::string line;
    std::vector<Edge> connections;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        connections.push_back({});
        auto& [from, to] = connections.back();
        sscanf(line.c_str(), "%c%c-%c%c", &from[0], &from[1], &to[0], &to[1]);
    }

    Graph graph;
    CreateGraph(connections, graph);

    std::cout << "    Part 1: " << FindGroups(graph, 3) << std::endl;
    std::cout << "    Part 2: " << FindLargestGroup(graph) << std::endl;
}
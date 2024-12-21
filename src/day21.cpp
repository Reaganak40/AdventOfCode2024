#include "pch.h"

/**
Physical Keypad:
+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
    | 0 | A |
    +---+---+ 
*/
// { x, y }
#define BUTTON_7 { 0, 0 }
#define BUTTON_8 { 1, 0 }
#define BUTTON_9 { 2, 0 }
#define BUTTON_4 { 0, 1 }
#define BUTTON_5 { 1, 1 }
#define BUTTON_6 { 2, 1 }
#define BUTTON_1 { 0, 2 }
#define BUTTON_2 { 1, 2 }
#define BUTTON_3 { 2, 2 }
#define BUTTON_0 { 1, 3 }
#define BUTTON_A { 2, 3 }


/*
Directional Keypad:
    +---+---+
    | ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+
*/

#define BUTTON_UP      { 1, 0 }
#define BUTTON_DOWN    { 1, 1 }
#define BUTTON_LEFT    { 0, 1 }
#define BUTTON_RIGHT   { 2, 1 }
#define BUTTON_A_DIR   { 2, 0 }

struct CharCharIntHash
{
    std::size_t operator()(const std::tuple<char, char, int>& p) const
    {
        std::hash<char> hasher;
        return hasher(std::get<0>(p)) ^ hasher(std::get<1>(p)) ^ std::get<2>(p);
    }
};

struct CharCharHash
{
    std::size_t operator()(const std::pair<char, char>& p) const
    {
        std::hash<char> hasher;
        return hasher(p.first) ^ hasher(p.second);
    }
};

void SolvePhysicalKeypad(const std::string& code, std::vector<std::string>& permutations)
{
    // start on postion 'A'
    int posx = 2, posy = 3;

    std::function <void(int&, int&, char, std::vector<std::string>&)> move = [&](int& x, int& y, char target, std::vector<std::string>& options)
    {
        std::pair<int, int> target_pos = { x, y };
        std::string opt1, opt2; // xy or yx for movement order

        switch (target)
        {
            case '0': target_pos = BUTTON_0; break;
            case '1': target_pos = BUTTON_1; break;
            case '2': target_pos = BUTTON_2; break;
            case '3': target_pos = BUTTON_3; break;
            case '4': target_pos = BUTTON_4; break;
            case '5': target_pos = BUTTON_5; break;
            case '6': target_pos = BUTTON_6; break;
            case '7': target_pos = BUTTON_7; break;
            case '8': target_pos = BUTTON_8; break;
            case '9': target_pos = BUTTON_9; break;
            case 'A': target_pos = BUTTON_A; break;
        }

        int dx = target_pos.first - x;
        int dy = target_pos.second - y;
        char x_dir = dx > 0 ? '>' : '<';
        char y_dir = dy > 0 ? 'v' : '^';

        // move x then y
        if (!(y == 3 && target_pos.first == 0)) {
            for (int i = 0; i < abs(dx); i++) opt1.push_back(x_dir);
            for (int i = 0; i < abs(dy); i++) opt1.push_back(y_dir);
            opt1.push_back('A');
        } 

        // move y then x
        if (!(x == 0 && target_pos.second == 3)) {
            for (int i = 0; i < abs(dy); i++) opt2.push_back(y_dir);
            for (int i = 0; i < abs(dx); i++) opt2.push_back(x_dir);
            opt2.push_back('A');
        }
        x = target_pos.first;
        y = target_pos.second;

        if (opt1.length() > 0) options.push_back(opt1);
        if (opt2.length() > 0) {
            if (opt1 != opt2) {
                options.push_back(opt2);
            }
        }
    };

    for (char c : code)
    {
        std::vector<std::string> n_moves;
        move(posx, posy, c, n_moves);

        size_t psize = permutations.size();
        if (psize == 0) {
            permutations = n_moves;
        } else {
            for (size_t i = 0; i < psize; i++) {
                std::string copy = permutations[i];
                permutations.at(i) += n_moves[0];
                if (n_moves.size() > 1) {
                    permutations.push_back(copy + n_moves[1]);
                }
            }
        }
    }
}

std::vector<std::string>& GetOptions(char from, char to)
{
    static std::unordered_map<std::pair<char, char>, std::shared_ptr<std::vector<std::string>>, CharCharHash> options;
    if (options.find({ from, to }) != options.end()) {
        return *options.at({ from, to });
    }

    options[{ from, to }] = std::make_shared<std::vector<std::string>>();

    std::pair<int, int> from_pos, to_pos;
    switch (from) {
        case 'A': from_pos = BUTTON_A_DIR; break;
        case '<': from_pos = BUTTON_LEFT; break;
        case '>': from_pos = BUTTON_RIGHT; break;
        case '^': from_pos = BUTTON_UP; break;
        case 'v': from_pos = BUTTON_DOWN; break;
        default: return *options[{ from, to }];
    };

    switch (to) {
        case 'A': to_pos = BUTTON_A_DIR; break;
        case '<': to_pos = BUTTON_LEFT; break;
        case '>': to_pos = BUTTON_RIGHT; break;
        case '^': to_pos = BUTTON_UP; break;
        case 'v': to_pos = BUTTON_DOWN; break;
        default: return *options[{ from, to }];
    };

    // note we only move all x then all y or vise versa. While other permutations
    // exist where we move x then y then x then y ... etc, these will always be longer
    // when the robots use the directional keypad
    
    int dx = to_pos.first - from_pos.first;
    int dy = to_pos.second - from_pos.second;

    std::string opt1, opt2; // xy or yx for movement order
    char x_dir = dx > 0 ? '>' : '<';
    char y_dir = dy > 0 ? 'v' : '^';

    // move x then y
    if (!(from_pos.second == 0 && to_pos.first == 0)) {
        for (int i = 0; i < abs(dx); i++) opt1.push_back(x_dir);
        for (int i = 0; i < abs(dy); i++) opt1.push_back(y_dir);
        opt1.push_back('A');
    }

    // move y then x
    if (!(from_pos.first == 0 && to_pos.second == 0)) {
        for (int i = 0; i < abs(dy); i++) opt2.push_back(y_dir);
        for (int i = 0; i < abs(dx); i++) opt2.push_back(x_dir);
        opt2.push_back('A');
    }

    if (opt1.length() > 0) options[{ from, to }]->push_back(opt1);
    if (opt2.length() > 0) {
        if (opt1 != opt2) {
            options[{ from, to }]->push_back(opt2);
        }
    }

    return *options.at({ from, to });
}

uint64_t SolveDirectionalKeypad(const std::vector<std::string>& perms, int max_depth)
{
    std::unordered_map<std::tuple<char, char, int>, uint64_t, CharCharIntHash> memo;

    std::function<uint64_t(char, char, int)> dfs = [&](char from, char to, int depth) -> uint64_t {
        if (memo.find({ from, to, depth }) != memo.end()) {
            return memo.at({ from, to, depth });
        }

        if (depth == max_depth) {
            return 1;
        }

        uint64_t lowest = std::numeric_limits<uint64_t>::max();
        for (const std::string& o : GetOptions(from, to)) {
            uint64_t total = 0;
            std::string perm = 'A' + o;
            
            for (int i = 0; i < perm.size() - 1; i++) {
                total += dfs(perm[i], perm[i + 1], depth + 1);
            }

            lowest = std::min(lowest, total);
        }

        // std::cout << "From: " << from << " To: " << to << " Depth: " << depth << " Total: " << lowest << std::endl;
        memo[{ from, to, depth }] = lowest;
        return lowest;
    };

    uint64_t lowest = std::numeric_limits<uint64_t>::max();
    for (const std::string& o : perms) {
        uint64_t total = 0;
        std::string perm = 'A' + o;
        for (int i = 0; i < perm.size() - 1; i++) {
            total += dfs(perm[i], perm[i + 1], 0);
        }

        lowest = std::min(lowest, total);
    }

    return lowest;
}

uint64_t GetNumericalPartOfCode(const std::string& code)
{
    std::string numerical_code;
    for (char c : code)
    {
        if (std::isdigit(c))
        {
            numerical_code.push_back(c);
        }
    }

    return std::stoull(numerical_code);
}

uint64_t Solve(const std::vector<std::string>& codes, int part = 1)
{
    uint64_t total = 0;
    const int robot_count = (part == 1) ? 2 : 25;

    for (const std::string& code : codes)
    {
        std::vector<std::string> current_perms;
        SolvePhysicalKeypad(code, current_perms); // get all possible permutations for the code (should be like 3 tops)

        // use dfs with memoize to find size of shortest path
        total += GetNumericalPartOfCode(code) * SolveDirectionalKeypad(current_perms, robot_count);
    }

    return total;
}

void DoDay21()
{
    std::cout << "Day 21" << std::endl;

    std::ifstream file("inputs/day21/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    // get input
    std::string line;
    std::vector<std::string> codes;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        codes.push_back(line);
    }

    std::cout << "    Part 1: " << Solve(codes, 1) << std::endl;
    std::cout << "    Part 2: " << Solve(codes, 2) << std::endl;
}
#include "pch.h"
#include "utils.hpp"

#define MAX_DEPTH 100

struct Game
{
    // [x, y]
    int buttonA[2];
    int buttonB[2];
    int prize[2];
};

struct GameState
{
    int x;
    int y;
    int depth_a;
    int depth_b;

    bool operator==(const GameState& other) const {
        return x == other.x && y == other.y && depth_a == other.depth_a && depth_b == other.depth_b;
    }
};

// hash for game state
struct TupleHashGameState {
    std::size_t operator()(const GameState& t) const {
        // Combine the hash values of the two integers
        return std::hash<int>()(t.x) ^ (std::hash<int>()(t.y) << 1) ^ (std::hash<int>()(t.depth_a) << 2) ^ (std::hash<int>()(t.depth_b) << 3);
    }

    bool operator()(const GameState& lhs, const GameState& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.depth_a == rhs.depth_a && lhs.depth_b == rhs.depth_b;
    }
};


uint64_t PlayGame(const Game& game)
{
    std::unordered_map<GameState, uint64_t, TupleHashGameState> memo_result;
    std::unordered_map<GameState, bool, TupleHashGameState> memo_won;


    std::function<uint64_t(GameState, bool&)> playGame = [&](GameState state, bool& won) -> uint64_t {

        if (memo_result.find(state) != memo_result.end()) {
            won = memo_won.at(state);
            return memo_result.at(state);
        }

        if (state.depth_a > MAX_DEPTH || state.depth_b > MAX_DEPTH) {
            won = false;
            memo_result[state] = 0;
            memo_won[state] = false;
            return 0;
        }

        if (state.x > game.prize[0] || state.y > game.prize[1]) {
            won = false;
            memo_result[state] = 0;
            memo_won[state] = false;
            return 0;
        }

        if (state.x == game.prize[0] && state.y == game.prize[1]) {
            memo_result[state] = 0;
            memo_won[state] = true;
            won = true;
            return 0;
        }

        bool buttonA_wins = false;
        uint64_t use_buttonA = 3 + playGame({state.x + game.buttonA[0], state.y + game.buttonA[1], state.depth_a + 1, state.depth_b}, buttonA_wins);

        bool buttonB_wins = false;
        uint64_t use_buttonB = 1 + playGame({state.x + game.buttonB[0], state.y + game.buttonB[1], state.depth_a, state.depth_b + 1}, buttonB_wins);

        uint64_t result = 0;
        if (buttonA_wins && buttonB_wins) {
            won = true;
            result = std::min(use_buttonA, use_buttonB);
        } else if (buttonA_wins) {
            won = true;
            result = use_buttonA;
        } else if (buttonB_wins) {
            won = true;
            result = use_buttonB;
        }

        memo_result[state] = result;
        memo_won[state] = won;
        return result;
    };

    bool won = false;
    return playGame({0, 0, 0, 0}, won);
}

uint64_t PlayGames(const std::vector<Game>& games)
{
    std::cout << std::endl;
    uint64_t total = 0;
    for (const auto& game : games) {
        std::cout << "Button A: " << game.buttonA[0] << ", " << game.buttonA[1] << std::endl;
        std::cout << "Button B: " << game.buttonB[0] << ", " << game.buttonB[1] << std::endl;
        std::cout << "Prize: " << game.prize[0] << ", " << game.prize[1] << std::endl;
        std::cout << "Result: ";

        uint64_t result = PlayGame(game);
        std::cout << "    " << result << std::endl;
        std::cout << std::endl;

        total += result;
    }

    return total;
}

void DoDay13()
{
    std::cout << "Day 13: " << std::endl;
    std::ifstream file("inputs/day13/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<Game> games;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        Game game;
        sscanf(line.c_str(), "Button A: X+%d, Y+%d", &game.buttonA[0], &game.buttonA[1]);
        std::getline(file, line);

        sscanf(line.c_str(), "Button B: X+%d, Y+%d", &game.buttonB[0], &game.buttonB[1]);
        std::getline(file, line);

        sscanf(line.c_str(), "Prize: X=%d, Y=%d", &game.prize[0], &game.prize[1]);
        games.push_back(game);
    }
    std::cout << "    Part 1: " << PlayGames(games) << std::endl;
}
#include "pch.h"
#include "utils.hpp"

#define MAX_DEPTH 100

struct Game
{
    // [x, y]
    uint64_t buttonA[2];
    uint64_t buttonB[2];
    uint64_t prize[2];
};

uint64_t PlayGame(const Game& game, bool use_max_depth)
{
    double Xa = (double)game.buttonA[0];
    double Xb = (double)game.buttonB[0];
    double Xp = -(double)game.prize[0];

    double Ya = (double)game.buttonA[1];
    double Yb = (double)game.buttonB[1];
    double Yp = -(double)game.prize[1];

    // calculate intersection
    double Na = ((Xb * Yp) - (Yb * Xp)) / ((Xa * Yb) - (Ya * Xb));
    double Nb = ((Ya * Xp) - (Xa * Yp)) / ((Xa * Yb) - (Ya * Xb));

    // check if not whole numbers
    if (Na != (int64_t)Na || Nb != (int64_t)Nb) {
        return 0;
    }

    // check if intersection is at negative coordinates
    if (Na < 0 || Nb < 0) {
        return 0;
    }

    // check if coordinates are too large
    if (use_max_depth) {
        if (Na > MAX_DEPTH || Nb > MAX_DEPTH) {
            return 0;
        }
    }

    // calculate cost
    return (3 * (uint64_t)(Na)) + ((uint64_t)(Nb));
}

uint64_t PlayGames(std::vector<Game>& games, int part = 1)
{
    uint64_t total = 0;

    for (auto& game : games) {
        uint64_t diff = part == 1 ? 0 : 10000000000000;
        game.prize[0] += diff;
        game.prize[1] += diff;

        total += PlayGame(game, part == 1);
        game.prize[0] -= diff;
        game.prize[1] -= diff;
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
        (void)sscanf(line.c_str(), "Button A: X+%lld, Y+%lld", &game.buttonA[0], &game.buttonA[1]);
        std::getline(file, line);


        (void)sscanf(line.c_str(), "Button B: X+%lld, Y+%lld", &game.buttonB[0], &game.buttonB[1]);
        std::getline(file, line);

        (void)sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &game.prize[0], &game.prize[1]);
        games.push_back(game);
    }
    
    std::cout << "    Part 1: " << PlayGames(games, 1) << std::endl;
    std::cout << "    Part 2: " << PlayGames(games, 2) << std::endl;
}
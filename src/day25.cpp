#include "pch.h"

#define ITEM_KEY 0
#define ITEM_LOCK 1

struct Item
{
    int vals[5] = {0, 0, 0, 0, 0};
};

void ReadInput(std::ifstream& file, std::vector<Item>& keys, std::vector<Item>& locks)
{
    std::string line;
    std::vector<std::string> rows;

    auto add_item = [&] () {
        if (rows.size() != 7) return;
        bool is_lock = rows[0][0] == '#';

        if (is_lock) {
            locks.push_back({});
            Item& lock = locks.back();

            for (int i = 1; i <= 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (rows[i][j] == '#') lock.vals[j]++;
                }
            }
        } else {
            keys.push_back({});
            Item& key = keys.back();

            for (int i = 5; i >= 1; i--) {
                for (int j = 0; j < 5; j++) {
                    if (rows[i][j] == '#') key.vals[j]++;
                }
            }
        }

        rows.clear();
    };

    while (std::getline(file, line)) {
        if (line.empty()) {
            add_item();
            continue;
        }
        rows.push_back(line);
    }

    add_item();
}

uint64_t FindMatches(std::vector<Item>& keys, std::vector<Item>& locks)
{
    uint64_t matches = 0;

    for (auto& key : keys) {
        for (auto& lock : locks) {
            bool match = true;
            for (int i = 0; i < 5; i++) {
                if (key.vals[i] + lock.vals[i] > 5) {
                    match = false;
                    break;
                }
            }
            if (match) matches++;
        }
    }

    return matches;
}

void DoDay25()
{
    std::cout << "Day 25" << std::endl;

    std::ifstream file("inputs/day25/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::vector<Item> keys;
    std::vector<Item> locks;
    ReadInput(file, keys, locks);

    std::cout << "    Part 1: " << FindMatches(keys, locks) << std::endl;
}
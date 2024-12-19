#include "pch.h"

struct StripeMap
{
    std::vector<std::string> stripes[26];

    void AddStripes(const std::vector<std::string>& strip_arr)
    {
        for (const auto& stripe : strip_arr) {
            int index = stripe[0] - 'a';
            this->stripes[index].push_back(stripe);
        }
    }

    void GetPossibleMatches(const char* pattern, std::vector<std::string>& matches) const
    {
        std::string pattern_str = pattern;
        int index = pattern_str[0] - 'a';

        for (const auto& stripe : stripes[index]) {
            if (stripe.length() > pattern_str.length()) {
                continue;
            }

            if (std::equal(stripe.begin(), stripe.end(), pattern_str.begin())) {
                matches.push_back(stripe);
            }
        }
    }
};

void Split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        // strip whitespace
        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
        elems.push_back(item);
    }
}

struct StringPairHash
{
    std::size_t operator()(const std::pair<std::string, std::string>& p) const
    {
        std::hash<std::string> hasher;
        return hasher(p.first) ^ hasher(p.second);
    }
};

uint64_t CountPossibleStripes(const StripeMap& sm, const std::vector<std::string>& patterns, int part)
{
    std::unordered_map<std::pair<std::string, std::string>, uint64_t, StringPairHash> memo;

    std::function<uint64_t(std::string, std::string)> IsMatch = [&](const std::string& pattern, std::string curr) -> uint64_t {
        if (memo.find({pattern, curr}) != memo.end()) {
            return memo.at({pattern, curr});
        }

        if (pattern.length() == curr.length()) {
            memo[{pattern, curr}] = 1;
            return 1;
        }

        int next = curr.length();
        std::vector<std::string> matches;

        sm.GetPossibleMatches(&pattern.c_str()[next], matches);
        if (matches.size() == 0) {
            memo[{pattern, curr}] = 0;
            return 0;
        }

        uint64_t sum = 0;
        for (const auto& match : matches) {
            sum += IsMatch(pattern, curr + match);
        }

        memo[{pattern, curr}] = sum;
        return sum;
    };

    uint64_t total_matches = 0;
    for (const auto& pattern : patterns) {
        uint64_t res = IsMatch(pattern, "");
        if (res > 0) {
            total_matches += (part == 1) ? 1 : res;
        }
    }

    return total_matches;
}

void DoDay19()
{
    std::cout << "Day 19" << std::endl;

    std::ifstream file("inputs/day19/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }


    // get input
    std::string line;

    StripeMap sm;
    std::vector<std::string> patterns;
    std::getline(file, line);
    Split(line, ',', patterns);
    sm.AddStripes(patterns);
    patterns.clear();
    
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        patterns.push_back(line);
    }

    std::cout << "    Part 1: " << CountPossibleStripes(sm, patterns, 1) << std::endl;
    std::cout << "    Part 2: " << CountPossibleStripes(sm, patterns, 2) << std::endl;
}
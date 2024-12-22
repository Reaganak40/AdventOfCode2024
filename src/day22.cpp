#include "pch.h"


struct Sequence
{
    // the change in numbers
    int s1, s2, s3, s4;
    int last_num;

    void AddToSequence(int num)
    {
        num %= 10;
        int ns = num - last_num;
        last_num = num;
        
        s1 = s2;
        s2 = s3;
        s3 = s4;
        s4 = ns;
    }

    uint32_t ConvertBits()
    {
        // since all possible values are -9 to 9, we can use 5 bits to represent each value
        // allows us to represent the sequence as an optimized 32 bit integer (for alignment purposes)
        uint32_t bits = 0;
        bits |= ((unsigned char)(s1 + 9)) << 20;
        bits |= ((unsigned char)(s2 + 9)) << 15;
        bits |= ((unsigned char)(s3 + 9)) << 10;
        bits |= ((unsigned char)(s4 + 9)) << 5;
        return bits;
    }
};

uint64_t GenSecretNumber(uint64_t secret_number)
{
    secret_number = ((secret_number << 6) ^ secret_number) & 0xffffff;
    secret_number = ((secret_number >> 5) ^ secret_number) & 0xffffff;
    return ((secret_number << 11) ^ secret_number) & 0xffffff;
}


uint64_t Solve1(const std::vector<uint64_t>& initial_nums)
{
    const int iterations = 2000;
    uint64_t sum = 0;

    for (uint64_t num : initial_nums){
        num = GenSecretNumber(num);
        for (int i = 1; i < iterations; i++) num = GenSecretNumber(num);
        sum += num;
    }

    return sum;

}

int Solve2(const std::vector<uint64_t>& initial_nums)
{
    const int iterations = 2000;

    // track running total of all sequences
    std::unordered_map<uint32_t, int> totals;

    for (uint64_t num : initial_nums) {
        Sequence seq = { 0, 0, 0, 0 };
        std::unordered_set<uint32_t> seen;

        seq.AddToSequence(num);
        for (int i = 1; i < iterations; i++) {
            num = GenSecretNumber(num);
            seq.AddToSequence(num);
            if (i < 4) continue;

            uint32_t bits = seq.ConvertBits();
            if (seen.find(bits) != seen.end()) continue;
            seen.insert(bits);

            if (totals.find(bits) == totals.end()) {
            totals[bits] = seq.last_num;
            } else {
                totals[bits] += seq.last_num;
            }
        }
    }

    // get the largest total
    int largest = 0;
    for (const auto& [seq, total] : totals) {
        largest = std::max(largest, total);
    }

    return largest;
}


void DoDay22()
{
    std::cout << "Day 22" << std::endl;

    std::ifstream file("inputs/day22/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    // get input
    std::string line;
    std::vector<uint64_t> nums;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        nums.push_back(std::stoull(line));
    }

    std::cout << "    Part 1: " << Solve1(nums) << std::endl;
    std::cout << "    Part 2: " << Solve2(nums) << std::endl;
}
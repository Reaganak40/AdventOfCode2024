#include "pch.h"


struct Sequence
{
    // the change in numbers
    int s1, s2, s3, s4;

    void AddToSequence(int num)
    {
        static int last_num = num;
        int ns = num - last_num;
        last_num = num;
        
        s1 = s2;
        s2 = s3;
        s3 = s4;
        s4 = ns;
    }

    bool operator==(const Sequence& other) const {
        return s1 == other.s1 && s2 == other.s2 && s3 == other.s3 && s4 == other.s4;
    }
};

struct SequenceHash
{
    std::size_t operator()(const Sequence& s) const {
        std::hash<int> hasher;
        return hasher(s.s1) ^ hasher(s.s2) ^ hasher(s.s3) ^ hasher(s.s4);
    }
};

uint64_t GenSecretNumber(uint64_t secret_number)
{
    uint64_t n_number = secret_number << 6;
    secret_number ^= n_number;
    secret_number %= 16777216;

    n_number = secret_number >> 5;
    secret_number ^= n_number;
    secret_number %= 16777216;

    n_number = secret_number << 11;
    secret_number ^= n_number;
    secret_number %= 16777216;

    return secret_number;
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
    std::unordered_map<Sequence, int, SequenceHash> totals;

    for (uint64_t num : initial_nums){
        Sequence seq = { 0, 0, 0, 0 };
        std::unordered_set<Sequence, SequenceHash> seen;

        seq.AddToSequence(num % 10);
        for (int i = 1; i < iterations; i++) {
            num = GenSecretNumber(num);
            int price = num % 10;
            seq.AddToSequence(price);

            if (i < 4) continue;
            if (seen.find(seq) != seen.end()) continue;
            seen.insert(seq);

            if (totals.find(seq) == totals.end()) {
                totals[seq] = price;
            } else {
                totals[seq] += price;
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
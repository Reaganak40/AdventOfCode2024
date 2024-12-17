#include "pch.h"
#include "utils.hpp"
#include "math.h"

struct Program
{
    uint64_t a;
    uint64_t b;
    uint64_t c;
    std::vector<uint64_t> instructions;
};

// opcodes
#define adv 0
#define bxl 1
#define bst 2
#define jnz 3
#define bxc 4
#define out 5
#define bdv 6
#define cdv 7

std::string RunProgram(Program program) {

    uint64_t curr_a = 1;
    uint64_t ip = 0;
    std::vector<uint64_t> output;
    output.reserve(program.instructions.size());

    auto get_combo_op = [&](uint64_t op) -> uint64_t {
        if (op <= 3) {
            return op;
        }
        switch (op) {
            case 4:
                return program.a;
            case 5:
                return program.b;
            case 6:
                return program.c;
            default:
                std::cout << "Invalid combo op: " << op << std::endl;
                return 0;
        };
    };

    ip = 0;
    while(ip < program.instructions.size()) {
        uint64_t opcode = program.instructions[ip];
        switch(opcode) {
            case adv:
                program.a = program.a / (uint64_t)pow(2, get_combo_op(program.instructions[ip + 1]));
                ip += 2;
                break;
            case bxl:
                program.b ^= program.instructions[ip + 1];
                ip += 2;
                break;
            case bst:
                program.b = get_combo_op(program.instructions[ip + 1]) % 8;
                ip += 2;
                break;
            case jnz:
                if (program.a != 0) {
                    ip = program.instructions[ip + 1];
                } else {
                    ip += 2;
                }
                break;
            case bxc:
                program.b ^= program.c;
                ip += 2;
                break;
            case out:
                output.push_back(get_combo_op(program.instructions[ip + 1]) % 8);
                ip += 2;
                break;
            case bdv:
                program.b = program.a / (uint64_t)pow(2, get_combo_op(program.instructions[ip + 1]));
                ip += 2;
                break;
            case cdv:
                program.c = program.a / (uint64_t)pow(2, get_combo_op(program.instructions[ip + 1]));
                ip += 2;
                break;
            default:
                std::cout << "Invalid opcode: " << opcode << std::endl;
                return "";
        }
       
    }

    std::string output_str;
    for (uint64_t i = 0; i < output.size(); i++) {
        output_str += std::to_string(output[i]);
        if (i != output.size() - 1) {
            output_str += ",";
        }
    }
    return output_str;
}

uint64_t ReverseProgram(Program program) {
    uint64_t goal = program.instructions.back();

    auto is_part_of_sequence = [&](int size, std::string& sequence) -> bool {
        std::vector<uint64_t> sequence_numbers;
        SplitIntoLargeNumbers(sequence, sequence_numbers);
        int idx = program.instructions.size() - size;
        for (int i = 0; i < sequence.size(); i++) {
            if (program.instructions[idx++] != sequence_numbers[i]) {
                return false;
            }
            if (idx == program.instructions.size()) {
                if (i == (sequence_numbers.size() - 1)) {
                    return true;
                }
                break;
            }
        }

        return false;
     };
    
    std::vector<std::pair<uint64_t, int>> nodes;
    for (uint64_t a = 1; a < 8; a++) {
        program.a = a;
        std::string res = RunProgram(program);
        if (is_part_of_sequence(1, res)) {
            nodes.push_back({ a, 1 });
        }
    }

    if (nodes.size() == 0) {
        std::cout << "Could not reverse program" << std::endl;
        return 0;
    }

    std::vector<uint64_t> results;
    while (!nodes.empty()) {
        auto [node, idx] = nodes.back();
        nodes.pop_back();
        for (uint64_t next_node = node * 8; ((next_node / 8)) == node; next_node++) {
            program.a = next_node;
            std::string res = RunProgram(program);
            if (is_part_of_sequence(idx + 1, res)) {
                if ((idx + 1) == program.instructions.size()) {
                    results.push_back(next_node);
                }
                else {
                    nodes.push_back({ next_node, idx + 1 });
                }
            }
        }
    }

    // sort results
    std::sort(results.begin(), results.end());
    return results[0];
}

void DoDay17()
{
    std::cout << "Day 17" << std::endl;

    std::ifstream file("inputs/day17/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    Program program;
    std::string line;
    
    // get program/input
    std::getline(file, line);
    (void)sscanf(line.c_str(), "Register A: %llu", &program.a);
    std::getline(file, line);
    (void)sscanf(line.c_str(), "Register B: %llu", &program.b);
    std::getline(file, line);
    (void)sscanf(line.c_str(), "Register C: %llu", &program.c);
    
    std::getline(file, line);
    std::getline(file, line);
    SplitIntoLargeNumbers(line, program.instructions);
    file.close();

    std::cout << "    Part 1: " << RunProgram(program) << std::endl;
    std::cout << "    Part 2: " << ReverseProgram(program) << std::endl;
}
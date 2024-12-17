#include "pch.h"
#include "utils.hpp"
#include "math.h"

struct Program
{
    int a;
    int b;
    int c;
    std::vector<int> instructions;
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

std::string RunProgram(Program program, int part = 1) {

    int curr_a = 1;
    
    start_program:
    if (part == 2) {
        program.a = curr_a;
    }

    int ip = 0;
    auto get_combo_op = [&](int op) -> int {
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

    std::string output = "";
    while(ip < program.instructions.size()) {
        int opcode = program.instructions[ip];
        switch(opcode) {
            case adv:
                program.a = program.a / (int)pow(2, get_combo_op(program.instructions[ip + 1]));
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
                if (output.size() > 0) {
                    output += ",";
                }
                output += std::to_string(get_combo_op(program.instructions[ip + 1]) % 8);
                ip += 2;
                break;
            case bdv:
                program.b = program.a / (int)pow(2, get_combo_op(program.instructions[ip + 1]));
                ip += 2;
                break;
            case cdv:
                program.c = program.a / (int)pow(2, get_combo_op(program.instructions[ip + 1]));
                ip += 2;
                break;
            default:
                std::cout << "Invalid opcode: " << opcode << std::endl;
                return "";
        }
    }

    if (part == 1) {
        return output;
    } else {

        std::vector<int> n_program;
        SplitIntoNumbers(output, n_program);

        // check if same as original program
        if (n_program.size() == program.instructions.size()) {
            for (int i = 0; i < n_program.size(); i++) {
                if (n_program[i] != program.instructions[i]) {
                    curr_a++;
                    goto start_program;
                }
            }
        } else {
            curr_a++;
            goto start_program;
        }
        
        return std::to_string(program.a);
    }
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
    sscanf(line.c_str(), "Register A: %d", &program.a);
    std::getline(file, line);
    sscanf(line.c_str(), "Register B: %d", &program.b);
    std::getline(file, line);
    sscanf(line.c_str(), "Register C: %d", &program.c);
    
    std::getline(file, line);
    std::getline(file, line);
    SplitIntoNumbers(line, program.instructions);
    file.close();

    std::cout << "    Part 1: " << RunProgram(program, 1) << std::endl;
    std::cout << "    Part 2: " << RunProgram(program, 2) << std::endl;
}
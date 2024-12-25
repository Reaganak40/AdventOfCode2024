#include "pch.h"

using WireArray = std::unordered_map<std::string, int>;
using InitalInput = std::pair<std::string, int>;

#define GATE_XOR 0
#define GATE_AND 1
#define GATE_OR 2

static void Split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


uint64_t BinaryToDecimal(const std::string& binary)
{
    uint64_t decimal = 0;
    for (size_t i = 0; i < binary.size(); i++) {
        decimal += (binary[i] - '0') * pow(2, binary.size() - i - 1);
    }
    return decimal;
}

std::string DecimalToBinary(uint64_t decimal)
{
    std::string binary;
    while (decimal > 0) {
        binary = std::to_string(decimal % 2) + binary;
        decimal /= 2;
    }
    return binary;
}

uint64_t CombineBits(const WireArray& wires, char bit_delim)
{
    std::vector<std::string> wire_names;
    std::string bits;

    for (const auto& [wire, value] : wires) {
        if (wire[0] == bit_delim) wire_names.push_back(wire);
    }

    std::sort(wire_names.begin(), wire_names.end(), [](const std::string& a, const std::string& b) {
        return a > b;
    });
    for (const auto& wire : wire_names) bits += std::to_string(wires.at(wire));

    return BinaryToDecimal(bits);
}

uint64_t GetWireBit(const std::string& wire)
{
    std::string bit;
    if (wire[0] != 'z' && wire[0] != 'x' && wire[0] != 'y') return 0;
    bit = wire.substr(1);
    return std::stoi(bit);
}

struct Gate
{
    std::string operand1;
    std::string operand2;
    std::string result;
    int operator_type;

    Gate() : operand1(""), operand2(""), result(""), operator_type(-1) {}

    bool IsEmpty() const { return operand1.empty(); }
    bool CanCompute(const WireArray& wires) const { return wires.find(operand1) != wires.end() && wires.find(operand2) != wires.end(); }

    void Compute(WireArray& wires) const
    {
        switch (operator_type) {
            case GATE_XOR:
                wires[result] = wires.at(operand1) ^ wires.at(operand2);
                break;
            case GATE_AND:
                wires[result] = wires.at(operand1) & wires.at(operand2);
                break;
            case GATE_OR:
                wires[result] = wires.at(operand1) | wires.at(operand2);
                break;
            default:
                std::cout << "Unknown gate: " << operator_type << std::endl;
                break;
        }
    }

    std::string ToString() const
    {
        std::string op;
        switch (operator_type) {
            case GATE_XOR:
                op = "XOR";
                break;
            case GATE_AND:
                op = "AND";
                break;
            case GATE_OR:
                op = "OR";
                break;
            default:
                op = "UNKNOWN";
                break;
        }

        return operand1 + " " + op + " " + operand2 + " -> " + result;
    }
};

struct WireSystem
{
    std::vector<InitalInput> initial_values;
    std::vector<Gate> gates;
    int num_wires;

    void ReadInput(std::ifstream& file)
    {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                break;
            }

            char wire[4];
            int value;
            sscanf(line.c_str(), "%3s: %d", wire, &value);
            initial_values.push_back({wire, value});
        }

        while (std::getline(file, line)) {
            if (line.empty()) {
                break;
            }

            std::vector<std::string> elems;
            Split(line, ' ', elems);
            if (elems.size() != 5) continue;

            Gate gate;
            gate.operand1 = elems[0];
            gate.operand2 = elems[2];
            gate.result   = elems[4];
            if (elems[1] == "XOR") {
                gate.operator_type = GATE_XOR;
            } else if (elems[1] == "AND") {
                gate.operator_type = GATE_AND;
            } else if (elems[1] == "OR") {
                gate.operator_type = GATE_OR;
            } else {
                std::cout << "Unknown gate: " << elems[1] << std::endl;
            }

            gates.push_back(gate);
        }
    }

    uint64_t ComputeZ() const
    {
        WireArray wires;
        for (const auto& [wire, value] : initial_values) {
            wires[wire] = value;
        }

        std::vector<int> tasks(gates.size(), 1);
        auto get_task = [&]() -> const Gate* {
            for (int i = 0; i < tasks.size(); i++) {
                if (tasks[i] == 0) continue;

                if (gates[i].CanCompute(wires)) {
                    tasks[i] = 0;
                    return &gates[i];
                }
            }

            return nullptr;
        };

        const Gate* task = nullptr;
        while ((task = get_task()) != nullptr) {
            task->Compute(wires);
        }

        return CombineBits(wires, 'z');
    }

    std::string Solve(int part)
    {
        if (part == 1) {
            return std::to_string(ComputeZ()); 
        }
        
        // get the max number of bits we need to add x and y
        int max_bit = 0;
        {
            WireArray wires;
            for (const auto& [wire, value] : initial_values) wires[wire] = value;
            std::string target = DecimalToBinary(CombineBits(wires, 'x') + CombineBits(wires, 'y'));
            max_bit = target.size();
        }

        // gets the gate that computes the result
        auto get_gate = [&](const std::string& result) -> const Gate* {
            for (const auto& gate : gates) {
                if (gate.result == result) return &gate;
            }

            std::cout << "Gate not found: " << result << std::endl;
            return nullptr;
        };

        // checks if a gate is an XOR gate with both operands being x or y and from the same expected bit
        auto is_xy_xor = [&] (const std::string& result, int bit) -> std::string {
            const Gate* gate = get_gate(result);
            if (gate == nullptr) return result;
            if (gate->operator_type != GATE_XOR) return result;
            if (gate->operand1[0] != 'x' && gate->operand1[0] != 'y') return result;
            if (gate->operand2[0] != 'x' && gate->operand2[0] != 'y') return result;
            if (GetWireBit(gate->operand1) != bit) return result;
            if (GetWireBit(gate->operand2) != bit) return result;
            return "";
        };

        // checks if a gate is an AND gate with both operands being x or y and from the same expected bit
        auto is_xy_and = [&] (const std::string& result, int bit) -> std::string {
            const Gate* gate = get_gate(result);
            if (gate == nullptr) return result;
            if (gate->operator_type != GATE_AND) return result;
            if (gate->operand1[0] != 'x' && gate->operand1[0] != 'y') return result;
            if (gate->operand2[0] != 'x' && gate->operand2[0] != 'y') return result;
            if (GetWireBit(gate->operand1) != bit) return result;
            if (GetWireBit(gate->operand2) != bit) return result;
            return "";
        };

        // checks Cn_1 = (Xn AND Yn) OR (Cn AND (Xn XOR Yn))
        std::function<std::string(const std::string&, int)> is_carry;

        // checks (Cn  AND (Xn XOR Yn))
        auto is_subcarry = [&] (const std::string& result, int bit) -> std::string {
            const Gate* gate = get_gate(result);
            if (gate == nullptr) return result;
            if (gate->operator_type !=  GATE_AND) return result;
            if (gate->operand1[0] == 'x') return result;
            if (gate->operand1[0] == 'y') return result;

            // empty string means the condition is met
            std::string cond1 = is_xy_xor(gate->operand1, bit);
            std::string cond2 = is_carry(gate->operand2, bit);
            if (cond1.empty() && cond2.empty()) return "";
            if (cond1.empty()) return cond2;
            if (cond2.empty()) return cond1;

            cond1 = is_carry(gate->operand1, bit);
            cond2 = is_xy_xor(gate->operand2, bit);
            if (cond1.empty() && cond2.empty()) return "";
            if (cond1.empty()) return cond2;
            if (cond2.empty()) return cond1;

            return result;
        };

        // checks Cn_1 = (Xn AND Yn) OR (Cn AND (Xn XOR Yn))
        is_carry = [&] (const std::string& result, int bit) -> std::string {
            const Gate* gate = get_gate(result);
            if (gate == nullptr) return result;
            if (gate->operator_type != GATE_OR) {
                if (bit != 1) return result;
                if (gate->operator_type != GATE_AND) return result;
                if (gate->operand1[0] != 'x' && gate->operand1[0] != 'y') return result;
                if (gate->operand2[0] != 'x' && gate->operand2[0] != 'y') return result;
                if (GetWireBit(gate->operand1) != 0) return result;
                if (GetWireBit(gate->operand2) != 0) return result;
                return "";
            }

            std::string cond1 = is_xy_and(gate->operand1, bit - 1);
            std::string cond2 = is_subcarry(gate->operand2, bit - 1);
            if (cond1.empty() && cond2.empty()) return "";
            if (cond1.empty()) return cond2;
            if (cond2.empty()) return cond1;

            cond1 = is_xy_and(gate->operand2, bit - 1);
            cond2 = is_subcarry(gate->operand1, bit - 1);
            if (cond1.empty() && cond2.empty()) return "";
            if (cond1.empty()) return cond2;
            if (cond2.empty()) return cond1;

            return result;
        };

        // checks if an output gate's structure matches a Ripple Carry Adder
        auto check_output = [&] (std::string zResult) -> std::string {
            const Gate* gate = get_gate(zResult);
            if (gate == nullptr) return zResult;
            if (gate->result[0] != 'z') return zResult;
            if (gate->operator_type != GATE_XOR) {
                if (GetWireBit(gate->result) == max_bit - 1) {
                    if (gate->operator_type == GATE_OR) return "";
                    return zResult;
                }
                if (GetWireBit(gate->result) == 0) return "";
                int bit = GetWireBit(gate->result);
                return is_xy_xor(gate->result, bit);
            }
            if (GetWireBit(gate->result) == 0) return "";
            int bit = GetWireBit(gate->result);
            
            std::string cond1 = is_xy_xor(gate->operand1, bit);
            std::string cond2 = is_carry(gate->operand2, bit);
            if (cond1.empty() && cond2.empty()) return "";
            if (cond1.empty()) return cond2;
            if (cond2.empty()) return cond1;

            cond1 = is_carry(gate->operand1, bit);
            cond2 = is_xy_xor(gate->operand2, bit);
            if (cond1.empty() && cond2.empty()) return "";
            if (cond1.empty()) return cond2;
            if (cond2.empty()) return cond1;

            return zResult;
        };
        
        std::unordered_set<std::string> swaps;
        for (int i = 36; i < max_bit; i++) {
            std::string z_val = "z" + std::to_string(i);
            if (z_val.length() == 2) {
                z_val = "z0" + std::to_string(i);
            }

            std::string check = check_output(z_val);
            if (!check.empty()) {
                if (check == "dkj") {
                    std::cout << "dkj at bit " << i << std::endl;
                }
                swaps.insert(check);
            }
        }

        std::vector<std::string> swaps_sorted;
        for (const auto& swap : swaps) {
            swaps_sorted.push_back(swap);
            std::cout << get_gate(swap)->ToString() << std::endl;

        }
        std::sort(swaps_sorted.begin(), swaps_sorted.end(), [](const std::string& a, const std::string& b) {
            return a < b;
        });

        std::string swap_str;
        for (int i = 0; i < swaps_sorted.size(); i++) {
            swap_str += swaps_sorted[i];
            if (i < swaps_sorted.size() - 1) {
                swap_str += ",";
            }
        }

        return swap_str;
    }
};

void DoDay24()
{
    std::cout << "Day 24" << std::endl;

    std::ifstream file("inputs/day24/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    // get input
    WireSystem wsystem;
    wsystem.ReadInput(file);

    std::cout << "    Part 1: " << wsystem.Solve(1) << std::endl;
    std::cout << "    Part 2: " << wsystem.Solve(2) << std::endl;
}
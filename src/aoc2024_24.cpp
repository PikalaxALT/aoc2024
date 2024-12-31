#include <deque>
#include <ranges>
#include <set>
#include <numeric>
#include "aoc2024.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

class Day24 : public aoc2024::Impl {
    struct command_t {
        std::string op;
        std::set<std::string> in;
        std::string out;
        friend std::ostream &operator<<(std::ostream &strm, const command_t &cmd) {
            aoc2024::print(strm, cmd.in, std::format(" {:s} ", cmd.op)) << " -> " << cmd.out;
            return strm;
        }
    };

    std::map<std::string, bool> wires_init;
    std::deque<command_t> commands;  // op, in1, in2, out
public:
    Day24 (const std::string &input) : aoc2024::Impl(input) {
        bool parsingCommands = false;
        for (auto line : std::string_view(input) | std::views::split("\n"sv)) {
            if (line.empty()) {
                parsingCommands = true;
            } else if (parsingCommands) {
                // inp1 OP inp2 -> out
                auto parts = std::views::split(line, " "sv)
                    | std::views::transform([](const auto &x) {
                        return std::string(x.begin(), x.end());
                    })
                    | std::ranges::to<std::vector>();
                if (parts.size() != 5) {
                    throw std::runtime_error("invalid command: "s + std::string(line.begin(), line.end()));
                }
                commands.push_back({parts[1], {parts[0], parts[2]}, parts[4]});
            } else {
                // name: state
                auto line_s = std::string(line.begin(), line.end());
                wires_init[line_s.substr(0, line_s.find(':'))] = line_s.back() == '1';
            }
        }
    }

    // pass by value because we don't want to modify the inputs from the caller
    static std::map<std::string, bool> process(std::map<std::string, bool> wires, std::deque<command_t> queue) {
        while (!queue.empty()) {
            auto [op, inputs, dest] = queue.front();
            queue.pop_front();
            auto map_its = std::views::transform(inputs, [&](const std::string &inp) {
                return wires.find(inp);
            });
            if (std::ranges::contains(map_its, wires.end())) {
                queue.emplace_back(op, inputs, dest);
                continue;
            }
            if (op == "AND") {
                wires[dest] = std::accumulate(map_its.begin(), map_its.end(), true, [](bool a, auto it) { return a & it->second; });
            } else if (op == "OR") {
                wires[dest] = std::accumulate(map_its.begin(), map_its.end(), false, [](bool a, auto it) { return a | it->second; });
            } else if (op == "XOR") {
                wires[dest] = std::accumulate(map_its.begin(), map_its.end(), false, [](bool a, auto it) { return a ^ it->second; });
            } else {
                throw std::runtime_error("invalid op");
            }
        }
        // std::cerr << '\n';
        return wires;
    }

    unsigned long long wires_to_value(const std::map<std::string, bool> &wires, char prefix) {
        unsigned long long result = 0;
        // This takes advantage of the language specification that iterating over a std::map
        // yields key-value pairs in key sorted order
        for (int bit = 0; auto [k, v] : std::ranges::subrange(
            wires.lower_bound(prefix + "00"s),
            wires.upper_bound(prefix + "64"s)
        )) {
            if (v) {
                result |= 1ull << bit;
            }
            ++bit;
        }
        return result;
    }

    void part1 () final {
        auto wires = process(wires_init, commands);
        unsigned long long result = wires_to_value(wires, 'z');
        std::cout << result << std::endl;
    }

    void part2 () final {
        if (isExample()) {
            std::cout << "The example is unusable" << std::endl;
            return;
        }
        using aoc2024::operator<<;
        auto wires = process(wires_init, commands);
        std::set<std::string> swaps;
        std::string lastOutput = std::ranges::max(commands, [](const auto &a, const auto &b) {return a.out < b.out;}).out;
        auto outputWidth = std::stoul(lastOutput.substr(1));
        // half adder:
        //   S = x ^ y
        //   C = x & y
        // full adder:
        //   d = x ^ y
        //   z = d ^ c
        //   e = x & y
        //   f = e & c
        //   g = e | f

        // Segregate the commands by operation
        struct bidir_maps {
            std::map<std::string, std::set<std::string>> out_to_in;
            std::map<std::set<std::string>, std::string> in_to_out;
        };
        std::map<std::string, bidir_maps> gates;
        for (const auto &cmd : commands) {
            gates[cmd.op].out_to_in[cmd.out] = cmd.in;
            gates[cmd.op].in_to_out[cmd.in] = cmd.out;
        }

        // Find all gates immediately downstream of inputs and immediately upstream of outputs
        std::string carry;
        for (unsigned i = 0; i < outputWidth; ++i) {
            std::string x_input = std::format("x{:02d}", i);
            std::string y_input = std::format("y{:02d}", i);
            std::string z_output = std::format("z{:02d}", i);
            std::string xy_xor = gates["XOR"].in_to_out.at({x_input, y_input});
            std::string xy_and = gates["AND"].in_to_out.at({x_input, y_input});
            if (!gates["XOR"].out_to_in.contains(z_output)) {
                swaps.insert(z_output);
            }
            if (i == 0) {
                if (xy_xor != z_output) {
                    swaps.insert(xy_xor);
                    swaps.insert(z_output);
                }
                carry = xy_and;
            } else {
                std::set<std::string> expectedInputs {xy_xor, carry};
                try {
                    std::string xyc_xor = gates["XOR"].in_to_out.at(expectedInputs);
                    if (xyc_xor != z_output) {
                        swaps.insert(xyc_xor);
                        swaps.insert(z_output);
                    }
                } catch (std::out_of_range) {
                    auto trueInputs = gates["XOR"].out_to_in.at(z_output);
                    std::array<std::string, 2> swapped {};
                    std::ranges::set_symmetric_difference(trueInputs, expectedInputs, swapped.begin());
                    swaps.insert_range(swapped);
                    if (swapped[0] == carry) {
                        carry = swapped[1];
                    } else if (swapped[1] == carry) {
                        carry = swapped[0];
                    }
                }
                expectedInputs = {xy_xor, carry};
                std::string xyc_and;
                try {
                    xyc_and = gates["AND"].in_to_out.at(expectedInputs);
                } catch (std::out_of_range) {
                    auto trueInputs = std::ranges::find_if(gates["AND"].in_to_out, [&](const auto &p) {
                        return p.first.contains(carry);
                    });
                    std::array<std::string, 2> swapped {};
                    std::ranges::set_symmetric_difference(trueInputs->first, expectedInputs, swapped.begin());
                    swaps.insert_range(swapped);
                    xyc_and = trueInputs->second;
                }
                expectedInputs = {xy_and, xyc_and};
                try {
                    carry = gates["OR"].in_to_out.at(expectedInputs);
                } catch (std::out_of_range) {
                    auto trueInputs = std::ranges::find_if(gates["OR"].in_to_out, [&](const auto &p) {
                        return p.first.contains(xy_and) || p.first.contains(xyc_and);
                    });
                    std::array<std::string, 2> swapped {};
                    std::ranges::set_symmetric_difference(trueInputs->first, expectedInputs, swapped.begin());
                    swaps.insert_range(swapped);
                    carry = trueInputs->second;
                }
            }
        }
        if (carry != lastOutput) {
            swaps.insert(carry);
            swaps.insert(lastOutput);
        }

        // Print the result
        std::cerr << swaps << std::endl;
    }
};

int main () {
    aoc2024::main<Day24>();
}

#include <deque>
#include <ranges>
#include <set>
#include "aoc2024.hpp"

using aoc2024::operator<<;

class Day24 : public aoc2024::Impl {
    std::map<std::string, bool> wires_init;
    std::vector<std::tuple<std::string, std::string, std::string, std::string>> commands;  // op, in1, in2, out
public:
    Day24 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        using std::operator""s;
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
                commands.push_back({parts[1], parts[0], parts[2], parts[4]});
            } else {
                // name: state
                auto line_s = std::string(line.begin(), line.end());
                wires_init[line_s.substr(0, line_s.find(':'))] = line_s.back() == '1';
            }
        }
    }

    std::map<std::string, bool> process() {
        std::map<std::string, bool> wires = wires_init;
        std::deque<decltype(commands)::value_type> queue (commands.begin(), commands.end());
        while (!queue.empty()) {
            auto [op, lhs, rhs, dest] = queue.front();
            queue.pop_front();
            if (!wires.contains(lhs) || !wires.contains(rhs)) {
                queue.push_back({op, lhs, rhs, dest});
            } else {
                if (op == "AND") {
                    wires[dest] = wires[lhs] & wires[rhs];
                } else if (op == "OR") {
                    wires[dest] = wires[lhs] | wires[rhs];
                } else if (op == "XOR") {
                    wires[dest] = wires[lhs] ^ wires[rhs];
                } else {
                    throw std::runtime_error("invalid op");
                }
                // std::cerr << lhs << "(" << wires[lhs] << ") " << op << " " << rhs << "(" << wires[rhs] << ") -> " << dest << "(" << wires[dest] << ")\n";
            }
        }
        // std::cerr << '\n';
        return wires;
    }

    void part1 () final {
        auto wires = process();
        unsigned long long result = 0;
        for (auto [k, v] : wires) {
            std::cerr << k << " = " << v << "\n";
            if (k.starts_with('z')) {
                unsigned bit = std::stoul(k.substr(1));
                result |= static_cast<unsigned long long>(v) << bit;
            }
        }
        std::cout << result << std::endl;
    }

    void part2 () final {
        auto wires = process();
        unsigned long long x = 0;
        unsigned long long y = 0;
        unsigned long long z = 0;
        for (auto [k, v] : wires) {
            if (k.starts_with('x')) {
                unsigned bit = std::stoul(k.substr(1));
                x |= static_cast<unsigned long long>(v) << bit;
            } else if (k.starts_with('y')) {
                unsigned bit = std::stoul(k.substr(1));
                y |= static_cast<unsigned long long>(v) << bit;
            } else if (k.starts_with('z')) {
                unsigned bit = std::stoul(k.substr(1));
                z |= static_cast<unsigned long long>(v) << bit;
            }
        }
        std::cerr << "x = " << x << "\n";
        std::cerr << "y = " << y << "\n";
        std::cerr << "x + y = " << (x + y) << "\n";
        std::cerr << "z = " << z << "\n";
        std::cerr << std::format("bad bits: {:b}\n", (x + y) ^ z);

        // sort by output wire
        std::ranges::sort(commands, [](const auto &a, const auto &b) {
            return std::get<3>(a) < std::get<3>(b);
        });
        // process all groups of eight output wires
    }
};

int main () {
    aoc2024::main<Day24>();
}

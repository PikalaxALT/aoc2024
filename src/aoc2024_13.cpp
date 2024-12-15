#include <regex>
#include "aoc2024.hpp"

class Day13 : public aoc2024::Impl {
    struct Machine {
        using coord2ull = std::pair<unsigned long long, unsigned long long>;
        coord2ull button_a;
        coord2ull button_b;
        coord2ull prize;
    };
    std::vector<Machine> machines;
public:
    Day13 (const std::string &input) : aoc2024::Impl(input) {
        std::regex pattern {R"(Button A: X\+(\d+), Y\+(\d+)\nButton B: X\+(\d+), Y\+(\d+)\nPrize: X=(\d+), Y=(\d+))"};
        for (std::smatch const &match : std::ranges::subrange(std::sregex_iterator(input.begin(), input.end(), pattern), std::sregex_iterator())) {
            machines.emplace_back(
                Machine::coord2ull{std::stoull(match[1]), std::stoull(match[2])},
                Machine::coord2ull{std::stoull(match[3]), std::stoull(match[4])},
                Machine::coord2ull{std::stoull(match[5]), std::stoull(match[6])}
            );
        }
    }

    unsigned long long getPrize (const Machine &machine, unsigned long long offset = 0) {
        auto prize_x = machine.prize.first + offset;
        auto prize_y = machine.prize.second + offset;
        long long denom = machine.button_a.first * machine.button_b.second - machine.button_a.second * machine.button_b.first;
        long long numer_a = prize_x * machine.button_b.second - prize_y * machine.button_b.first;
        long long numer_b = machine.button_a.first * prize_y - machine.button_a.second * prize_x;
        if (numer_a % denom == 0 && numer_b % denom == 0) {
            return (numer_a / denom) * 3 + (numer_b / denom);
        }
        return 0;
    }

    void part1 () final {
        unsigned long long result = 0ull;
        for (const auto &machine : machines) {
            result += getPrize(machine);
        }
        std::cout << result << std::endl;
    }

    void part2 () final {
        unsigned long long result = 0ull;
        for (const auto &machine : machines) {
            result += getPrize(machine, 10000000000000);
        }
        std::cout << result << std::endl;
    }
};

int main () {
    aoc2024::main<Day13>();
}

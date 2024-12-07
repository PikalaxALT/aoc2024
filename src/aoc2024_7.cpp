#include <map>
#include <vector>
#include <string_view>
#include <ranges>
#include "aoc2024.hpp"

class Day7 : public aoc2024::Impl {
    std::vector<std::pair<unsigned long long, std::vector<unsigned long long>>> operators;
    template <std::ranges::range Range>
    std::string parse(unsigned long long target, Range const& operands, bool concat = false) {
        std::string result {};
        if (operands.empty()) {
            return target == 0 ? "\n" : "";
        }
        if (operands.size() == 1) {
            return operands[0] == target ? "\n" : "";
        }
        std::vector<unsigned long long> temp (operands.begin() + 1, operands.end());
        temp[0] = operands[0] * operands[1];
        result = parse(target, temp, concat);
        if (!result.empty()) {
            return "*" + std::to_string(operands[1]) + result;
        } else {
            temp[0] = operands[0] + operands[1];
            result = parse(target, temp, concat);
            if (!result.empty()) {
                return "+" + std::to_string(operands[1]) + result;
            } else if (concat) {
                temp[0] = std::stoull(std::to_string(operands[0]) + std::to_string(operands[1]));
                result = parse(target, temp, concat);
                if (!result.empty()) {
                    return "||" + std::to_string(operands[1]) + result;
                }
            }
        }
        return result;
    }
public:
    Day7 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        for (const auto &line : std::string_view(input) | std::views::split("\n"sv) | std::views::transform([](const auto &x) {return std::string_view(x);})) {
            const auto spl_1 = line | std::views::split(": "sv) | std::views::transform([](const auto &x) {return std::string_view(x);}) | std::ranges::to<std::vector>();
            operators.emplace_back(std::stoull(std::string(spl_1[0])), spl_1[1]
                | std::views::split(" "sv)
                | std::views::transform([](const auto &x) {return std::string_view(x);})
                | std::views::transform([](const auto &x) {return std::string(x);})
                | std::views::transform([](const auto &x) {return std::stoull(x);})
                | std::ranges::to<std::vector>());
        }
    }

    void part1 () final {
        unsigned long long total = 0;
        for (const auto &[key, value] : operators) {
            std::string result = parse(key, value);
            if (!result.empty()) {
                std::cerr << key << ": " << value[0] << result;
                total += key;
            }
        }
        std::cout << total << std::endl;
    }

    void part2 () final {
        unsigned long long total = 0;
        for (const auto &[key, value] : operators) {
            std::string result = parse(key, value, true);
            if (!result.empty()) {
                std::cerr << key << ": " << value[0] << result;
                total += key;
            }
        }
        std::cout << total << std::endl;
    }
};

int main () {
    aoc2024::main<Day7>();
}

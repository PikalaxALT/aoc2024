#include <ranges>
#include <vector>
#include <algorithm>
#include <map>
#include "aoc2024.hpp"

class Day1 : public aoc2024::Impl {
    std::vector<int> left;
    std::vector<int> right;

    void parse (std::string const &input) {
        using std::operator""sv;
        const int nlines = std::ranges::count(input, '\n');

        left.clear();
        right.clear();

        left.reserve(nlines);
        right.reserve(nlines);

        for (const auto& line : std::views::split(std::string_view(input), "\n"sv)) {
            if (line.empty()) {
                break;
            }
            const std::string_view &view {line};
            auto row = std::views::split(view, " "sv) | std::ranges::to<std::vector<std::string>>();
            left.push_back(std::stoi(row.front()));
            right.push_back(std::stoi(row.back()));
        }

        std::ranges::sort(left);
        std::ranges::sort(right);
    }

public:
    void part1 (std::string const &input) override {
        parse(input);
        int total_distance = 0;
        for (const auto [l, r] : std::views::zip(left, right)) {
            total_distance += std::abs(l - r);
        }

        std::cout << total_distance << "\n";
    }

    void part2 (std::string const &input) override {
        parse(input);

        int total_similarity = 0;
        std::map<int, int> counter {};
        for (const int x : right) {
            ++counter[x];
        }
        for (const int x : left) {
            total_similarity += x * counter[x];
        }
        std::cout << total_similarity << "\n";
    }
};

int main () {
    aoc2024::main<Day1>();
}

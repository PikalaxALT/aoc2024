#include <numeric>
#include <ranges>
#include "aoc2024.hpp"

class Day19 : public aoc2024::Impl {
    std::vector<std::string_view> targets;
    std::vector<std::string_view> available;
public:
    Day19 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        if (input.empty()) {
            return;
        }
        auto lines = std::string_view(input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::ranges::to<std::vector>();
        available = lines[0]
            | std::views::split(", "sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::ranges::to<std::vector>();
        targets = lines
            | std::views::drop(2)
            | std::ranges::to<std::vector>();
    }

    unsigned long long isPossible(const std::string_view &target) {
        std::vector<unsigned long long> dp (target.size() + 1);
        dp[0] = 1;
        for (size_t i = 0; i < target.size(); ++i) {
            auto suffix = target.substr(i);
            for (const auto &avail : available) {
                if (suffix.starts_with(avail)) {
                    dp[i + avail.size()] += dp[i];
                }
            }
        }
        return dp[target.size()];
    }

    void part1 () final {
        auto result = std::accumulate(targets.begin(), targets.end(), 0ull, [this](auto a, const auto &target) {
            return a + (isPossible(target) != 0ull);
        });
        std::cout << result << std::endl;
    }

    void part2 () final {
        auto result = std::accumulate(targets.begin(), targets.end(), 0ull, [this](auto a, const auto &target) {
            return a + isPossible(target);
        });
        std::cout << result << std::endl;
    }
};

int main () {
    aoc2024::main<Day19>();
}

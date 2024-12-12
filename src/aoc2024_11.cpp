#include <ranges>
#include <array>
#include "aoc2024.hpp"

constexpr std::array<unsigned long long, 20> powersOf10 {{
    1ull,
    10ull,
    100ull,
    1000ull,
    10000ull,
    100000ull,
    1000000ull,
    10000000ull,
    100000000ull,
    1000000000ull,
    10000000000ull,
    100000000000ull,
    1000000000000ull,
    10000000000000ull,
    100000000000000ull,
    1000000000000000ull,
    10000000000000000ull,
    100000000000000000ull,
    1000000000000000000ull,
    10000000000000000000ull,
}};

constexpr unsigned ilog10(unsigned long long x) {
    return std::ranges::find_if_not(powersOf10, [=](unsigned long long p) {
        return p <= x;
    }) - powersOf10.begin();
}

constexpr unsigned long long ipow10(unsigned exponent) {
    return powersOf10[exponent];
}

class Day11 : public aoc2024::Impl {
    std::vector<unsigned long long> stones;

    constexpr std::vector<unsigned long long> blinkStone(unsigned long long x) const {
        if (x == 0) {
            return {1};
        } else if (ilog10(x) & 1) {
            return {x * 2024ull};
        } else {
            auto div = ipow10(ilog10(x) / 2);
            return { x / div, x % div };
        }
    }

    unsigned long long do_blinks(int remainingBlinks) const {
        static std::unordered_map<unsigned long long, std::vector<unsigned long long>> memo;
        std::unordered_map<unsigned long long, unsigned long long> counts;
        for (auto x : stones) {
            ++counts[x];
        }
        for (int i = 0; i < remainingBlinks; ++i) {
            auto counts_copy = counts;
            counts.clear();
            for (auto &[x, c] : counts_copy) {
                auto &ref = memo[x];
                if (ref.empty()) {
                    ref = blinkStone(x);
                }
                for (auto y : ref) {
                    counts[y] += c;
                }
            }
        }
        unsigned long long result = 0;
        for (auto [x, c] : counts) {
            result += c;
        }
        return result;
    }
public:
    Day11 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        stones = std::string_view(input)
            | std::views::split(" "sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::views::transform([](const auto &x) { return std::string(x); })
            | std::views::transform([](const auto &x) { return std::stoull(x); })
            | std::ranges::to<std::vector>();
    }

    void part1 () final {
        std::cout << do_blinks(25) << std::endl;
    }

    void part2 () final {
        std::cout << do_blinks(75) << std::endl;
    }
};

int main () {
    aoc2024::main<Day11>();
}

#include <ranges>
#include <numeric>
#include <unordered_set>
#include <deque>
#include "aoc2024.hpp"

class Day11 : public aoc2024::Impl {
    std::vector<unsigned long long> stones;

    constexpr unsigned ndigits(unsigned long long x, unsigned base = 10) const {
        unsigned result = 1;
        while (x >= base) {
            ++result;
            x /= base;
        }
        return result;
    }

    constexpr unsigned long long pow(unsigned exponent, unsigned base = 10) const {
        unsigned long long result = 1;
        for (unsigned i = 0; i < exponent; ++i) {
            result *= base;
        }
        return result;
    }

    constexpr std::vector<unsigned long long> blinkStone(unsigned long long x) const {
        if (x == 0) {
            return {1};
        } else if (ndigits(x) & 1) {
            return {x * 2024ull};
        } else {
            auto div = pow(ndigits(x) / 2);
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

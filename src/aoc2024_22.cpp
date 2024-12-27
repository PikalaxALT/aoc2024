#include <ranges>
#include <charconv>
#include "aoc2024.hpp"

class Day22 : public aoc2024::Impl {
    std::vector<unsigned long long> initialSecretNumbers;
public:
    Day22 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        initialSecretNumbers = std::string_view(input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) -> unsigned long long {
                unsigned long long ret;
                std::from_chars(x.begin(), x.end(), ret);
                return ret;
            })
            | std::ranges::to<std::vector>();
    }

    void part1 () final {
        unsigned long long result = 0;
        for (auto x : initialSecretNumbers) {
            for (int i = 0; i < 2000; ++i) {
                x = (x ^ (x << 6)) & 0xFFFFFF;
                x = (x ^ (x >> 5)) & 0xFFFFFF;
                x = (x ^ (x << 11)) & 0xFFFFFF;
            }
            result += x;
        }
        std::cout << result << std::endl;
    }

    void part2 () final {}
};

int main () {
    aoc2024::main<Day22>();
}

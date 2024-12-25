#include <ranges>
#include <numeric>
#include "aoc2024.hpp"

class Day25 : public aoc2024::Impl {
    std::vector<std::array<int, 5>> locks;
    std::vector<std::array<int, 5>> keys;
public:
    Day25 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""s;
        using std::operator""sv;
        auto lines = std::views::split(input, "\n"s)
            | std::views::transform([](const auto &x) { return std::string_view(x); });
        for (auto it = lines.begin(); it != lines.end();) {
            auto eit = std::ranges::find(it, lines.end(), ""sv);
            auto block = std::ranges::subrange(it, eit)
                | std::ranges::to<std::vector>();
            if (block[0][0] == '#') {
                auto &lock = locks.emplace_back();
                for (int i = 0; int &pin : lock) {
                    pin = std::ranges::find_if(block, [&](const auto &l) {return l[i] != '#';}) - block.begin() - 1;
                    ++i;
                }
            } else {
                auto &key = keys.emplace_back();
                for (int i = 0; int &height : key) {
                    height = std::ranges::find_if(std::views::reverse(block), [&](const auto &l) {return l[i] != '#';}) - block.rbegin() - 1;
                    ++i;
                }
            }
            if (eit == lines.end()) {
                break;
            }
            it = std::next(eit);
        }
    }

    void part1 () final {
        unsigned long long count = std::accumulate(locks.begin(), locks.end(), 0ull, [this](unsigned long long acc, const auto &lock) {
            return acc + std::ranges::count_if(keys, [&](const auto &key) {
                return std::ranges::all_of(
                    std::views::zip(lock, key),
                    [](const auto &t) {
                        auto [l, k] = t;
                        return l + k <= 5;
                    }
                );
            });
        });
        std::cout << count << std::endl;
    }

    void part2 () final {}
};

int main () {
    aoc2024::main<Day25>();
}

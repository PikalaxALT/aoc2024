#include <ranges>
#include <charconv>
#include <map>
#include <set>
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

    constexpr unsigned long long evolve(unsigned long long x) const {
        x = (x ^ (x << 6)) & 0xFFFFFF;
        x = (x ^ (x >> 5)) & 0xFFFFFF;
        x = (x ^ (x << 11)) & 0xFFFFFF;
        return x;
    }

    void part1 () final {
        unsigned long long result = 0;
        for (auto x : initialSecretNumbers) {
            for (int i = 0; i < 2000; ++i) {
                x = evolve(x);
            }
            result += x;
        }
        std::cout << result << std::endl;
    }

    void part2 () final {
        std::map<std::array<int, 4>, unsigned long long> patterns;
        for (int i = 0; auto x : initialSecretNumbers) {
            std::set<std::array<int, 4>> seen;
            std::array<int, 4> sequence;
            std::array<int, 5> mod10s;
            mod10s[0] = x % 10;
            int j;
            for (j = 0; j < 4; ++j) {
                x = evolve(x);
                mod10s[j + 1] = x % 10;
                sequence[j] = mod10s[j + 1] - mod10s[j];
            }
            for (; j < 2000; ++j) {
                if (seen.insert(sequence).second) {
                    patterns[sequence] += mod10s[4];
                }
                std::move(sequence.begin() + 1, sequence.end(), sequence.begin());
                std::move(mod10s.begin() + 1, mod10s.end(), mod10s.begin());
                x = evolve(x);
                mod10s[4] = x % 10;
                sequence[3] = mod10s[4] - mod10s[3];
            }
            ++i;
        }
        auto kvpair = std::ranges::max(std::views::keys(patterns), [&](const auto &a, const auto &b) {
            return patterns.at(a) < patterns.at(b);
        });
        using aoc2024::operator<<;
        std::cerr << "{" << kvpair << "}\n";
        std::cout << patterns.at(kvpair) << std::endl;
    }
};

int main () {
    aoc2024::main<Day22>();
}

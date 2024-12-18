#include <charconv>
#include <string>
#include <vector>
#include <ranges>
#include <string_view>
#include "aoc2024.hpp"

class Day2 : public aoc2024::Impl {
    std::vector<std::vector<int>> modules;

    bool isUnsafe(const std::vector<int> &module) {
        if (module.size() < 2) {
            return false;
        }
        int diff = module[1] - module[0];
        if (diff == 0 || std::abs(diff) > 3) {
            return true;
        }
        for (int i = 1; i < module.size() - 1; ++i) {
            int curdiff = module[i + 1] - module[i];
            if (curdiff * diff < 1 || std::abs(curdiff) > 3) {
                return true;
            }
        }
        return false;
    }
public:
    Day2 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        size_t nmodules = std::ranges::count(input, '\n') + 1;
        modules.reserve(nmodules);
        for (const auto &line : std::views::split(std::string_view(input), "\n"sv)) {
            auto &level = modules.emplace_back();
            level.reserve(std::ranges::count(line, ' ') + 1);
            for (const auto &val : std::views::split(std::string_view(line), " "sv)) {
                std::from_chars(val.begin(), val.end(), level.emplace_back());
            }
        }
    }

    void part1 () final {
        int nsafe = 0;
        for (const auto &module : modules) {
            nsafe += isUnsafe(module);
        }
        std::cout << nsafe << std::endl;
    }

    void part2 () final {
        int nsafe = 0;
        for (const auto &module : modules) {
            if (isUnsafe(module)) {
                ++nsafe;
                continue;
            }
            auto cur = module;
            cur.erase(cur.begin());
            for (size_t i = 0; i <= cur.size(); ++i) {
                if (isUnsafe(cur)) {
                    ++nsafe;
                    break;
                }
                if (i != cur.size()) {
                    cur[i] = module[i];
                }
            }
        }
        std::cout << nsafe << std::endl;
    }
};

int main() {
    aoc2024::main<Day2>();
}

#include <vector>
#include <map>
#include <set>
#include <ranges>
#include <algorithm>
#include "aoc2024.hpp"

class Day5 : public aoc2024::Impl {
    std::vector<std::array<int, 2>> rules;
    std::vector<std::vector<int>> updates;
    std::map<int, std::set<int>> illegals;
public:
    Day5 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        int state = 0;
        for (const auto &line : std::views::split(std::string_view(input), "\n"sv)) {
            auto line_sv = std::string_view(line);
            if (line_sv.empty()) {
                ++state;
            } else if (state == 0) {
                size_t pipe = line_sv.find('|');
                rules.push_back({std::stoi(std::string(line_sv.substr(0, pipe))), std::stoi(std::string(line_sv.substr(pipe + 1)))});
            } else {
                auto &update = updates.emplace_back();
                for (const auto &x : std::views::split(line_sv, ","sv)) {
                    update.push_back(std::stoi(std::string(std::string_view(x))));
                }
            }
        }
        for (const auto &rule : rules) {
            illegals[rule[1]].insert(rule[0]);
        }
    }

    void part1 () final {
        int total = 0;
        for (const auto &update : updates) {
            int i = 1;
            bool legal = true;
            for (auto x : std::ranges::subrange(update.begin(), update.end() - 1)) {
                auto it = illegals.find(x);
                if (it != illegals.end()) {
                    std::vector<int> result;
                    std::ranges::set_intersection(std::ranges::subrange(update.begin() + i, update.end()), it->second, std::back_inserter(result));
                    if (!result.empty()) {
                        legal = false;
                        break;
                    }
                }
                ++i;
            }
            if (legal) {
                total += update[update.size() / 2];
            }
        }
        std::cout << total << std::endl;
    }

    void part2 () final {
        int total = 0;
        for (const auto &update : updates) {
            bool legal = true;
            int i = 1;
            for (auto x : std::ranges::subrange(update.begin(), update.end() - 1)) {
                auto it = illegals.find(x);
                if (it != illegals.end()) {
                    std::vector<int> result;
                    std::ranges::set_intersection(std::ranges::subrange(update.begin() + i, update.end()), it->second, std::back_inserter(result));
                    if (!result.empty()) {
                        legal = false;
                        break;
                    }
                }
                ++i;
            }
            if (!legal) {
                // Attempt to fix the illegalities
                auto cur_update = update;
                while (!legal) {
                    legal = true;
                    i = 1;
                    for (auto x : std::ranges::subrange(cur_update.begin(), cur_update.end() - 1)) {
                        auto it = illegals.find(x);
                        if (it != illegals.end()) {
                            std::vector<int> result;
                            auto sr = std::ranges::subrange(cur_update.begin() + i, cur_update.end()) | std::ranges::to<std::vector>();
                            std::ranges::set_intersection(sr, it->second, std::back_inserter(result));
                            if (!result.empty()) {
                                legal = false;
                                for (auto it2 = cur_update.end() - 1; it2 + 1 != cur_update.begin(); --it2) {
                                    if (std::ranges::find(result, *it2) != result.end()) {
                                        cur_update.insert(it2 + 1, x);
                                        cur_update.erase(cur_update.begin() + i - 1);
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        ++i;
                    }
                }
                total += cur_update[cur_update.size() / 2];
            }
        }
        std::cout << total << std::endl;
    }
};

int main () {
    aoc2024::main<Day5>();
}

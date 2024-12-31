#include <ranges>
#include <map>
#include <deque>
#include <set>
#include "aoc2024.hpp"

class Day20 : public aoc2024::Impl {
    using coordui2_t = std::array<unsigned, 2>;
    std::vector<std::string_view> grid;
    coordui2_t start;
    coordui2_t end;
    std::array<std::array<int, 2>, 4> directions {{
        { -1,  0 },
        {  0,  1 },
        {  1,  0 },
        {  0, -1 },
    }};
    coordui2_t top_left;
    coordui2_t bottom_right;
public:
    Day20 (const std::string &input) : aoc2024::Impl(input) {
        if (input.empty()) {
            return;
        }
        using std::operator""sv;
        grid = std::string_view(input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::ranges::to<std::vector>();
        start[1] = std::ranges::find_if(grid, [this](const auto &x) {
            return (start[0] = x.find('S')) != static_cast<unsigned>(std::string::npos);
        }) - grid.begin();
        end[1] = std::ranges::find_if(grid, [this](const auto &x) {
            return (end[0] = x.find('E')) != static_cast<unsigned>(std::string::npos);
        }) - grid.begin();
    }

    std::map<coordui2_t, unsigned long long> solve(const coordui2_t &cheat = {-1u, -1u}) {
        std::map<coordui2_t, unsigned long long> dp;
        std::deque<coordui2_t> leadingEdge {start};
        while (!dp.contains(end) && !leadingEdge.empty()) {
            auto pos = leadingEdge.front();
            leadingEdge.pop_front();
            for (const auto &dir : directions) {
                coordui2_t npos {{ pos[0] + dir[0], pos[1] + dir[1] }};
                if (grid[npos[1]][npos[0]] == '#' && npos != cheat) {
                    continue;
                }
                if (!dp.contains(npos)) {
                    leadingEdge.push_back(npos);
                    dp[npos] = dp[pos] + 1;
                    if (npos == end) {
                        break;
                    }
                }
            }
        }
        return dp;
    }

    unsigned long long puzzle(int max_cheat) {
        auto path = solve();
        int threshold = isExample() ? (max_cheat == 2 ? 20 : 50) : 100;
        std::set<std::pair<coordui2_t, coordui2_t>> cheats;
        std::map<int, int> savings;
        for (const auto &[pos, score] : path) {
            // Fan out up to max_cheat units away
            std::set<coordui2_t> seen;
            std::set<coordui2_t> leadingEdge {pos};
            for (int i = 0; i < max_cheat; ++i) {
                std::set<coordui2_t> nextStack;
                for (const auto &cp : leadingEdge) {
                    for (const auto &dir : directions) {
                        coordui2_t np {cp[0] + dir[0], cp[1] + dir[1]};
                        if (np[0] >= grid[0].size() || np[1] >= grid.size() || !seen.insert(np).second) {
                            continue;
                        }
                        if (path.contains(np)) {
                            unsigned cheatScore = score + i + 1;
                            if (path[np] >= cheatScore + threshold) {
                                ++savings[path[np] - cheatScore];
                                cheats.insert({pos, np});
                            }
                        }
                        nextStack.insert(np);
                    }
                }
                leadingEdge = nextStack;
            }
        }
        if (isExample()) {
            for (auto [s, c] : savings) {
                std::cerr << std::format("{:d} cheats save {:d}ps\n", c, s);
            }
        }
        return cheats.size();
    }

    void part1 () final {
        std::cout << puzzle(2) << std::endl;
    }

    void part2 () final {
        std::cout << puzzle(20) << std::endl;
    }
};

int main () {
    aoc2024::main<Day20>();
}

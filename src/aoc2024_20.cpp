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
        std::cerr << "Path has " << path.size() << " nodes\n";
        unsigned long long count = 0;
        unsigned long long threshold = isExample() ? (max_cheat == 2 ? 20 : 50) : 100;
        for (const auto &[pos, score] : path) {
            std::deque<std::vector<coordui2_t>> cheatingEdge {{pos}};
            std::set<coordui2_t> seenEnds {};
            while (!cheatingEdge.empty()) {
                auto curCheat = cheatingEdge.front();
                cheatingEdge.pop_front();
                auto length = curCheat.size();
                if (length > max_cheat) {
                    continue;
                }
                auto ppos = curCheat.back();
                for (const auto &dir : directions) {
                    coordui2_t npos { ppos[0] + dir[0], ppos[1] + dir[1] };
                    if (std::ranges::contains(curCheat, npos)) {
                        continue;
                    }
                    auto curExtend = curCheat;
                    curExtend.push_back(npos);
                    if (path.contains(npos)) {
                        if (!seenEnds.insert(npos).second) {
                            continue;
                        }
                        if (path[npos] <= path[pos] + length) {
                            continue;
                        }
                        if (path[npos] - (path[pos] + length) >= threshold) {
                            ++count;
                        }
                        continue;
                    }
                    cheatingEdge.push_back(curExtend);
                }
            }
        }
        return count;
    }

    void part1 () final {
        std::cout << puzzle(2) << std::endl;
    }

    void part2 () final {
        // not solved in a reasonable amount of time
        // std::cout << puzzle(20) << std::endl;
    }
};

int main () {
    aoc2024::main<Day20>();
}

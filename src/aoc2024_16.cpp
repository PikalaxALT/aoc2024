#include "aoc2024.hpp"
#include <ranges>
#include <map>
#include <algorithm>

class Day16 : public aoc2024::Impl {
    static constexpr std::array<std::array<int, 2>, 4> directions {{
        {{  0, -1 }}, // north
        {{  1,  0 }}, // east
        {{  0,  1 }}, // south
        {{ -1,  0 }}, // west
    }};
    static constexpr std::string directionChars {"^>v<"};
    std::array<int, 2> start;
    std::array<int, 2> end;
    std::vector<std::string> grid;
public:
    Day16 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        grid = std::string_view(input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::views::transform([](const auto &x) { return std::string(x); })
            | std::ranges::to<std::vector>();
        start[1] = std::ranges::find_if(grid, [this](const auto &l) { return (start[0] = l.find('S')) != std::string::npos; }) - grid.begin();
        end[1] = std::ranges::find_if(grid, [this](const auto &l) { return (end[0] = l.find('E')) != std::string::npos; }) - grid.begin();
    }

    unsigned long long maze_solve(int x, int y, bool isPart2 = false) {
        std::vector<std::array<int, 2>> leadingEdge { start };
        std::vector<std::vector<std::tuple<unsigned long long, int, std::map<std::array<int, 2>, unsigned long long>>>> dp (grid.size());
        std::ranges::for_each(dp, [this](auto &row) {
            row.assign(grid[0].size(), {-1ull, -1, {}});
        });
        dp[start[1]][start[0]] = {0, 1, {}};

        for (int i = 0; i < leadingEdge.size(); ++i) {
            auto [x, y] = leadingEdge[i];
            if (x == end[0] && y == end[1]) {
                continue;
            }
            auto &[pscore, dir, tb] = dp[y][x];
            for (int turn = -1; turn <= 1; ++turn) {
                int cd = (dir + turn) & 3;
                auto [dx, dy] = directions[cd];
                if (grid[y + dy][x + dx] == '#') {
                    continue;
                }
                unsigned long long score = pscore + 1 + 1000 * (turn != 0);
                auto &[dscore, ddir, dtrace] = dp[y + dy][x + dx];
                if (score <= dscore) {
                    dscore = score;
                    ddir = cd;
                    dtrace[{x, y}] = pscore;
                    leadingEdge.push_back({x + dx, y + dy});
                }
            }
        }
        if (isPart2) {
            std::vector<std::array<int, 2>> trailingEdge {end};
            for (int i = 0; i < trailingEdge.size(); ++i) {
                auto [x, y] = trailingEdge[i];
                auto [cs, cd, bt] = dp[y][x];
                grid[y][x] = 'O';
                for (auto [pos, pscore] : bt) {
                    if (pscore < cs) {
                        trailingEdge.push_back(pos);
                    }
                }
            }
            return trailingEdge.size();
        } else {
            auto [cx, cy] = end;
            while (cx != start[0] || cy != start[1]) {
                auto [cs, cd, tb] = dp[cy][cx];
                auto [cdx, cdy] = directions[cd];
                cx -= cdx;
                cy -= cdy;
                grid[cy][cx] = directionChars[cd];
            }
            return std::get<0>(dp[end[1]][end[0]]);
        }
    }

    void part1 () final {
        std::cout << maze_solve(start[0], start[1]) << std::endl;
        std::ranges::for_each(grid, [](const auto &row) {
            std::cerr << row << std::endl;
        });
    }

    void part2 () final {
        std::ranges::for_each(grid, [this](auto &row) {
            std::ranges::for_each(directionChars, [&](auto c) {
                std::ranges::replace(row, c, '.');
            });
        });
        grid[start[1]][start[0]] = 'S';
        grid[end[1]][end[0]] = 'E';
        std::cout << maze_solve(start[0], start[1], true) << std::endl;
        std::ranges::for_each(grid, [](const auto &row) {
            std::cerr << row << std::endl;
        });
    }
};

int main () {
    aoc2024::main<Day16>();
}

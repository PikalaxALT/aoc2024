#include "aoc2024.hpp"
#include <ranges>
#include <deque>
#include <map>
#include <algorithm>

class Day16 : public aoc2024::Impl {
    using coordi2_t = std::array<int, 2>;

    static constexpr std::array<coordi2_t, 4> directions {{
        {{  0, -1 }}, // north
        {{  1,  0 }}, // east
        {{  0,  1 }}, // south
        {{ -1,  0 }}, // west
    }};
    static constexpr std::string directionChars {"^>v<"};
    coordi2_t start;
    coordi2_t end;
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

    unsigned maze_solve(bool isPart2 = false) {
        // The strategy is to find the best path, then find all corners leading there
        std::deque<coordi2_t> leadingEdge {start};
        std::vector<std::vector<std::tuple<coordi2_t, unsigned, unsigned>>> dp (grid.size());
        std::ranges::for_each(dp, [this](auto &row) {
            row.resize(grid[0].size(), {{-1, -1}, 4, -1u});
        });
        dp[start[1]][start[0]] = {start, 1, 0};
        std::map<std::pair<coordi2_t, coordi2_t>, std::map<coordi2_t, unsigned>> corners;

        while (!leadingEdge.empty()) {
            coordi2_t pos;
            auto [x, y] = pos = leadingEdge.front();
            leadingEdge.pop_front();
            auto [ppos, pdir, pscore] = dp[y][x];
            if (pos == end) {
                corners[{pos, pos}][ppos] = pscore;
                continue;
            }
            for (int turn = -1; turn <= 1; ++turn) {
                unsigned ndir = (pdir + turn + 4) % 4;
                auto [dx, dy] = directions[ndir];
                int nx = x + dx;
                int ny = y + dy;
                if (grid[ny][nx] == '#') {  // wall, do not enter
                    continue;
                }
                unsigned nscore = pscore + 1 + 1000 * (turn != 0);
                coordi2_t npos {nx, ny};
                auto &[rnpos, rndir, rnscore] = dp[ny][nx];
                if (nscore <= rnscore) {
                    rnpos = pos;
                    rndir = ndir;
                    rnscore = nscore;
                    leadingEdge.push_back(npos);
                }
                corners[{npos, pos}][ppos] = nscore;
            }
        }

        unsigned finalScore = std::get<2>(dp[end[1]][end[0]]);
        if (!isPart2) {
            return finalScore;
        }

        std::deque<std::tuple<coordi2_t, coordi2_t, unsigned>> trailingEdge {{end, end, finalScore}};

        unsigned result = 0;
        while (!trailingEdge.empty()) {
            auto [npos, pos, score] = trailingEdge.front();
            trailingEdge.pop_front();
            ++result;
            if (pos == start) {
                continue;
            }
            const auto &tb = corners.at({npos, pos});
            for (const auto &[ppos, pscore] : tb) {
                if (pscore != score) {
                    continue;
                }
                unsigned ppscore = score;
                if (pos != end) {
                    coordi2_t dir2 { npos[0] - pos[0], npos[1] - pos[1] };
                    coordi2_t dir1 { pos[0] - ppos[0], pos[1] - ppos[1] };
                    ppscore -= 1 + 1000 * (dir1 != dir2);
                }
                trailingEdge.push_back({pos, ppos, ppscore});
            }
        }

        return result;
    }

    void part1 () final {
        std::cout << maze_solve(false) << std::endl;
    }

    // Not complete
    // void part2 () final {
    //     std::cout << maze_solve(true) << std::endl;
    // }
};

int main () {
    aoc2024::main<Day16>();
}

#include "aoc2024.hpp"
#include <ranges>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

class Day16 : public aoc2024::Impl {
    using coordi2_t = std::array<int, 2>;

    enum Direction : uint8_t {
        south,
        east,
        north,
        west,
        max,
    };
    std::array<std::array<Direction, 3>, 4> possibleTurns {{
        { west, east },
        { south, north },
    }};
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
        unsigned result = 0;
        using node_t = std::tuple<unsigned, coordi2_t, Direction>;

        // Priority queue for leadingEdge. use greater<node_t> to pop the smallest element first
        std::priority_queue<node_t, std::vector<node_t>, std::greater<node_t>> leadingEdge;
        leadingEdge.emplace(0, start, Direction::east);
        std::vector<std::vector<std::array<unsigned, Direction::max>>> shortestDistance (grid.size());
        for (auto &row : shortestDistance) {
            row.assign(grid[0].size(), {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX});
        }
        shortestDistance[start[1]][start[0]][Direction::east] = 0;
        std::map<std::pair<coordi2_t, Direction>, std::set<std::pair<coordi2_t, Direction>>> backTrace {{{start, Direction::east}, {}}};

        while (!leadingEdge.empty()) {
            auto [score, pos, dir] = leadingEdge.top();
            leadingEdge.pop();
            if (score > shortestDistance[pos[1]][pos[0]][dir]) {
                continue;
            }

            auto tryInsert = [&](const coordi2_t &npos, Direction ndir, unsigned nscore) -> void {
                auto comp = (nscore <=> shortestDistance[npos[1]][npos[0]][ndir]);
                if (comp == std::strong_ordering::greater) {
                    return;
                }
                auto &bt = backTrace[{npos, ndir}];
                if (comp == std::strong_ordering::less) {
                    bt.clear();
                }
                bt.emplace(pos, dir);
                if (comp == std::strong_ordering::less) {
                    leadingEdge.emplace(nscore, npos, ndir);
                }
                shortestDistance[npos[1]][npos[0]][ndir] = nscore;
            };

            // Handle turns
            for (auto ndir : possibleTurns[dir % 2]) {
                tryInsert(pos, ndir, score + 1000);
            }
            // Handle step
            coordi2_t npos { pos[0] + directions[dir][0], pos[1] + directions[dir][1] };
            if (grid[npos[1]][npos[0]] != '#') {
                tryInsert(npos, dir, score + 1);
            }
        }

        result = std::ranges::min(shortestDistance[end[1]][end[0]]);

        if (isPart2) {
            std::set<coordi2_t> trailingEdge {end};
            std::set<std::pair<coordi2_t, Direction>> curStack;
            for (Direction dir = Direction::south; dir != Direction::max; dir = static_cast<Direction>(dir + 1)) {
                if (shortestDistance[end[1]][end[0]][dir] == result) {
                    curStack.emplace(end, dir);
                }
            }
            while (!curStack.empty()) {
                std::set<std::pair<coordi2_t, Direction>> nextStack;
                for (const auto &[pos, dir] : curStack) {
                    trailingEdge.insert(pos);
                    nextStack.insert_range(backTrace.at({pos, dir}));
                }
                curStack = nextStack;
            }
            result = trailingEdge.size();
        }

        return result;
    }

    void part1 () final {
        std::cout << maze_solve(false) << std::endl;
    }

    // Not complete
    void part2 () final {
        std::cout << maze_solve(true) << std::endl;
    }
};

int main () {
    aoc2024::main<Day16>();
}

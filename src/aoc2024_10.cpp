#include <set>
#include <ranges>
#include "aoc2024.hpp"

class Day10 : public aoc2024::Impl {
    std::vector<std::vector<int>> mountain;
    std::vector<std::vector<bool>> visited;
    static constexpr std::array<std::array<int, 2>, 4> directions {{
        {{  0, -1 }},
        {{  1,  0 }},
        {{  0,  1 }},
        {{ -1,  0 }},
    }};
    int trailScore(int x, int y, bool handle_visited = true) {
        int height = mountain[y][x];
        int score = 0;
        if (height == 9) {
            score = !handle_visited || !visited[y][x];
            visited[y][x] = true;
            return score;
        }
        for (auto [dx, dy] : directions) {
            int cx = x + dx;
            int cy = y + dy;
            if (cy >= 0 && cy < mountain.size() && cx >= 0 && cx < mountain[0].size() && mountain[cy][cx] - mountain[y][x] == 1) {
                score += trailScore(cx, cy, handle_visited);
            }
        }
        return score;
    }
public:
    Day10 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        mountain = std::views::split(std::string_view(input), "\n"sv)
            | std::views::transform([](const auto &x) {
                return x
                    | std::views::transform([](const auto c) { return c - '0'; })
                    | std::ranges::to<std::vector>();
            })
            | std::ranges::to<std::vector>();
        visited.resize(mountain.size());
        std::ranges::for_each(visited, [this](auto &row) {row.resize(mountain[0].size());});
    }

    void part1 () final {
        int score = 0;
        for (int y = 0; y < mountain.size(); ++y) {
            for (int x = 0; x < mountain[0].size(); ++x) {
                if (mountain[y][x] == 0) {
                    int curTrailheadScore = trailScore(x, y);
                    score += curTrailheadScore;
                    std::ranges::for_each(visited, [this](auto &row) {row.assign(mountain[0].size(), false);});
                }
            }
        }
        std::cout << score << "\n";
    }

    void part2 () final {
        int score = 0;
        for (int y = 0; y < mountain.size(); ++y) {
            for (int x = 0; x < mountain[0].size(); ++x) {
                if (mountain[y][x] == 0) {
                    int curTrailheadScore = trailScore(x, y, false);
                    score += curTrailheadScore;
                    std::ranges::for_each(visited, [this](auto &row) {row.assign(mountain[0].size(), false);});
                }
            }
        }
        std::cout << score << "\n";
    }
};

int main () {
    aoc2024::main<Day10>();
}

#include <ranges>
#include <charconv>
#include <deque>
#include "aoc2024.hpp"

class Day18 : public aoc2024::Impl {
    using coordui2_t = std::array<unsigned, 2>;
    std::vector<std::string> layout;
    unsigned getDim() const {
        return isExample() ? 7 : 71;
    }
    unsigned toRecord(bool isPart2 = false) const {
        if (isPart2) {
            return -1u;
        }
        return isExample() ? 12 : 1024;
    }
    bool hasWalls = false;
    std::string input;
    std::array<std::array<int, 2>, 4> directions {{
        { -1,  0 },
        {  0,  1 },
        {  1,  0 },
        {  0, -1 },
    }};
public:
    Day18 (const std::string &m_input) : aoc2024::Impl(m_input), input(m_input) {}

    void parse(bool isPart2 = false) {
        using std::operator""sv;
        layout.resize(getDim());
        std::ranges::for_each(layout, [this](auto &v) {
            v.assign(getDim(), '.');
        });
        hasWalls = !input.empty();
        for (const auto &line : std::string_view(input) | std::views::split("\n"sv) | std::views::take(toRecord(isPart2))) {
            auto coord = line
                | std::views::split(","sv)
                | std::views::transform([](const auto &x) {
                    unsigned v;
                    std::from_chars(x.begin(), x.end(), v);
                    return v;
                })
                | std::ranges::to<std::vector<unsigned>>();
            assert(coord.size() == 2);
            layout[coord[1]][coord[0]] = '#';
            if (isPart2 && solve() == -1u) {
                using aoc2024::operator<<;
                std::cout << coord << std::endl;
                break;
            }
        }
    }

    unsigned solve() {
        std::deque<coordui2_t> leadingEdge {{0, 0}};
        std::vector<std::vector<unsigned>> dp (getDim());
        std::ranges::for_each(dp, [this](auto &r) {
            r.assign(getDim(), -1u);
        });
        dp[0][0] = 0;
        while (!leadingEdge.empty()) {
            auto pos = leadingEdge.front();
            leadingEdge.pop_front();
            if (pos == coordui2_t{getDim() - 1, getDim() - 1}) {
                continue;
            }
            auto score = dp[pos[1]][pos[0]] + 1;
            for (const auto &dir : directions) {
                coordui2_t npos {{pos[0] + dir[0], pos[1] + dir[1]}};
                if (npos[0] >= getDim() || npos[1] >= getDim() || layout[npos[1]][npos[0]] == '#') {
                    continue;
                }
                if (dp[npos[1]][npos[0]] == -1u) {
                    dp[npos[1]][npos[0]] = score;
                    leadingEdge.push_back(npos);
                }
            }
        }
        return dp[getDim() - 1][getDim() - 1];

    }

    void part1 () final {
        parse();
        if (!hasWalls) {
            std::cout << "data not found\n";
            return;
        }

        std::cout << solve() << std::endl;
    }

    void part2 () final {
        parse(true);
    }
};

int main () {
    aoc2024::main<Day18>();
}

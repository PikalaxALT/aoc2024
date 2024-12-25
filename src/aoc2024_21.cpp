#include <ranges>
#include <map>
#include <set>
#include <numeric>
#include <charconv>
#include "aoc2024.hpp"

class Day21 : public aoc2024::Impl {
    const std::map<char, std::pair<int, int>> numpadCoords {
        {'0', {3, 1}},
        {'1', {2, 0}},
        {'2', {2, 1}},
        {'3', {2, 2}},
        {'4', {1, 0}},
        {'5', {1, 1}},
        {'6', {1, 2}},
        {'7', {0, 0}},
        {'8', {0, 1}},
        {'9', {0, 2}},
        {'A', {3, 2}},
        {'#', {3, 0}},
    };
    const std::map<char, std::pair<int, int>> dpadCoords {
        {'<', {1, 0}},
        {'>', {1, 2}},
        {'^', {0, 1}},
        {'v', {1, 1}},
        {'A', {0, 2}},
        {'#', {0, 0}},
    };
    const std::array<std::string_view, 4> numpad {{
        "987",
        "654",
        "321",
        "#0A",
    }};
    const std::array<std::string_view, 2> dpad {{
        "#^A",
        "<v>",
    }};
    std::vector<std::string_view> codes;

    std::vector<std::string> getRobotRoutes(char start, char end, std::map<char, std::pair<int, int>> const& coordsMap) {
        if (start == end) {
            return {"A"};
        }
        const auto &start_pos = coordsMap.at(start);
        const auto &end_pos = coordsMap.at(end);
        const auto &banPos = coordsMap.at('#');
        int dx = end_pos.second - start_pos.second;
        int dy = end_pos.first - start_pos.first;
        char lr = dx < 0 ? '<' : '>';
        char ud = dy < 0 ? '^' : 'v';
        auto route = std::string(std::abs(dx), lr) + std::string(std::abs(dy), ud);
        std::vector<std::string> ans;
        do {
            size_t first_turn = route.find_first_not_of(route.front());
            if (first_turn == std::string::npos) {
                first_turn = route.size();
            }
            if (start_pos.first == banPos.first && end_pos.second == banPos.second && route[0] == lr && first_turn == dx) {
                continue;
            }
            if (start_pos.second == banPos.second && end_pos.first == banPos.first && route[0] == ud && first_turn == dy) {
                continue;
            }
            ans.push_back(route + "A");
        } while (std::ranges::next_permutation(route).found);
        if (ans.empty()) {
            using std::operator""s;
            throw std::runtime_error("no route from "s + start + " to " + end);
        }
        return ans;
    }

    aoc2024::cached<std::vector<std::string>, char, char> getNumpadRoutes {[this](char start, char end) -> std::vector<std::string> {
        return getRobotRoutes(start, end, numpadCoords);
    }};

    aoc2024::cached<std::vector<std::string>, char, char> getDpadRoutes {[this](char start, char end) -> std::vector<std::string> {
        return getRobotRoutes(start, end, dpadCoords);
    }};

    aoc2024::cached<std::string, char, char> getBestDpadRoute {[this](char start, char end) -> std::string {
        return std::ranges::min(getDpadRoutes(start, end), [](const std::string &a, const std::string &b) {
            return a.size() < b.size();
        });
    }};

    aoc2024::cached<std::string, char, char> route {[this](char start, char end) -> std::string {
        using std::operator""s;
        auto compare_string_lengths = [](const std::string &a, const std::string &b) {
            return a.size() < b.size();
        };

        std::vector<std::string> numpadRoutes = getNumpadRoutes(start, end);
        std::set<std::string> dpad1Routes;
        for (const auto &rte : numpadRoutes) {
            char prev = 'A';
            std::string ret;
            for (const char c : rte) {
                std::vector<std::string> dpad2Routes = getDpadRoutes(prev, c);

            }
        }
    }};

    std::string get_route(const std::string_view &code) {
        using std::operator""s;
        char prev = 'A';
        return std::accumulate(code.begin(), code.end(), ""s, [&](const std::string &a, char c) {
            std::string ret = a + route(prev, c);
            prev = c;
            return ret;
        });
    }

public:
    Day21 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""s;
        codes = std::views::split(input, "\n"s)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::ranges::to<std::vector>();
    }

    void part1 () final {
        // This crashes. I'm too smoothbrained to fix.
        unsigned long long comp = 0ull;
        for (const auto &code : codes) {
            // get value
            unsigned long long value;
            std::from_chars(code.begin(), code.end() - 1, value);

            // get commands
            auto ans = get_route(code);
            std::cerr << code << ": " << ans << std::endl;

            // final complexity
            comp += ans.size() * value;
        }
        std::cout << comp << std::endl;
    }

    void part2 () final {}
};

int main () {
    aoc2024::main<Day21>();
}

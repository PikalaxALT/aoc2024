#include <vector>
#include <string>
#include <ranges>
#include <set>
#include <string_view>
#include "aoc2024.hpp"

class Day6 : public aoc2024::Impl {
    std::vector<std::string> layout;
    static constexpr std::array<std::array<int, 2>, 4> directions {{
        {{  0, -1 }},
        {{  1,  0 }},
        {{  0,  1 }},
        {{ -1,  0 }},
    }};
    static constexpr std::string dir_s { "^>v<" };

        size_t initial_x;
        size_t initial_y;
        int initialDirection;
public:
    Day6 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        layout = std::string_view(input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::views::transform([](const auto &x) { return std::string(x); })
            | std::ranges::to<std::vector>();
        initial_y = std::find_if(layout.begin(), layout.end(), [this](const std::string &row) {
            return (initial_x = row.find_first_of(dir_s)) != std::string::npos;
        }) - layout.begin();
        initialDirection = dir_s.find(layout[initial_y][initial_x]);
    }

    void part1 () final {
        // Get the guard's starting position and facing direction
        auto layout_copy = layout;
        ssize_t y = initial_y;
        ssize_t x = initial_x;
        int direction = initialDirection;
        layout_copy[y][x] = 'X';
        int count = 1;

        while (true) {
            ssize_t next_x = x + directions[direction][0];
            ssize_t next_y = y + directions[direction][1];
            if (next_x < 0 || next_x >= layout_copy[0].size() || next_y < 0 || next_y >= layout_copy.size()) {
                break;
            }
            if (layout_copy[next_y][next_x] == '#') {
                direction = (direction + 1) % dir_s.size();
            } else {
                count += layout_copy[next_y][next_x] != 'X';
                layout_copy[next_y][next_x] = 'X';
                y = next_y;
                x = next_x;
            }
        }

        std::cout << count << std::endl;
    }

    void part2 () final {
        int count = 0;
        for (size_t bruteforce_y = 0; bruteforce_y < layout.size(); ++bruteforce_y) {
            for (size_t bruteforce_x = 0; bruteforce_x < layout[0].size(); ++bruteforce_x) {
                if (layout[bruteforce_y][bruteforce_x] != '.') {
                    continue;
                }
                auto layout_copy = layout;
                ssize_t y = initial_y;
                ssize_t x = initial_x;
                int direction = initialDirection;
                layout_copy[bruteforce_y][bruteforce_x] = '#';
                layout_copy[y][x] = 'X';
                std::set<std::tuple<ssize_t, ssize_t, int>> history {{y, x, direction}};

                while (true) {
                    ssize_t next_x = x + directions[direction][0];
                    ssize_t next_y = y + directions[direction][1];
                    if (next_x < 0 || next_x >= layout_copy[0].size() || next_y < 0 || next_y >= layout_copy.size()) {
                        break;
                    }
                    if (layout_copy[next_y][next_x] == '#') {
                        direction = (direction + 1) % dir_s.size();
                    } else {
                        auto check = std::make_tuple(next_y, next_x, direction);
                        if (history.find(check) != history.end()) {
                            ++count;
                            break;
                        }
                        history.insert(check);
                        layout_copy[next_y][next_x] = 'X';
                        y = next_y;
                        x = next_x;
                    }
                }
            }
        }
        std::cout << count << std::endl;
    }
};

int main () {
    aoc2024::main<Day6>();
}

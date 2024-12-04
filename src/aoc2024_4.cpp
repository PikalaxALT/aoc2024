#include <string>
#include <vector>
#include <array>
#include <string_view>
#include <ranges>
#include "aoc2024.hpp"

class Day4 : public aoc2024::Impl {
    std::vector<std::string> grid;
public:
    Day4 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        grid.reserve(std::ranges::count(input, '\n') + 1);
        for (const auto &line : std::views::split(std::string_view(input), "\n"sv)) {
            grid.push_back(std::string(std::string_view(line)));
        }
    }

    void part1 () final {
        size_t nrows = grid.size();
        size_t ncols = grid[0].size();
        int count = 0;
        constexpr std::array<std::array<int, 2>, 8> directions {{
            {  0,  1 },
            {  1,  1 },
            {  1,  0 },
            {  1, -1 },
            {  0, -1 },
            { -1, -1 },
            { -1,  0 },
            { -1,  1 },
        }};
        for (size_t i = 0; i < nrows; ++i) {
            for (size_t j = 0; j < ncols && (j = grid[i].find('X', j)) != std::string::npos; ++j) {
                for (const auto [dy, dx] : directions) {
                    if (i + 3 * dy >= 0 && i + 3 * dy < nrows && j + 3 * dx >= 0 && j + 3 * dx < ncols
                    && grid[i + dy][j + dx] == 'M'
                    && grid[i + 2 * dy][j + 2 * dx] == 'A'
                    && grid[i + 3 * dy][j + 3 * dx] == 'S'
                    ) {
                        ++count;
                    }
                }
            }
        }
        std::cout << count << std::endl;
    }

    void part2 () final {
        size_t nrows = grid.size();
        size_t ncols = grid[0].size();
        int count = 0;
        constexpr std::array<std::array<int, 2>, 8> directions {{
            {  0,  1 },
            {  1,  1 },
            {  1,  0 },
            {  1, -1 },
            {  0, -1 },
            { -1, -1 },
            { -1,  0 },
            { -1,  1 },
        }};
        for (size_t i = 1; i < nrows - 1; ++i) {
            for (size_t j = 1; j < ncols - 1 && (j = grid[i].find('A', j)) != std::string::npos; ++j) {
                if (
                    ((grid[i - 1][j - 1] == 'M' && grid[i + 1][j + 1] == 'S') || (grid[i - 1][j - 1] == 'S' && grid[i + 1][j + 1] == 'M')) &&
                    ((grid[i - 1][j + 1] == 'M' && grid[i + 1][j - 1] == 'S') || (grid[i - 1][j + 1] == 'S' && grid[i + 1][j - 1] == 'M'))
                ) {
                    ++count;
                }
            }
        }
        std::cout << count << std::endl;
    }
};

int main () {
    aoc2024::main<Day4>();
}

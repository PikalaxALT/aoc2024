#include <numeric>
#include <regex>
#include "aoc2024.hpp"

class Day14 : public aoc2024::Impl {
    static constexpr unsigned width = 101;
    static constexpr unsigned height = 103;
    struct Robot {
        int x;
        int y;
        int dx;
        int dy;
    };
    std::vector<Robot> robots;
    long long safetyFactor(int steps) const {
        const int mwidth = isExample() ? 11 : width;
        const int mheight = isExample() ? 7 : height;
        std::array<int, 4> quadrants {};
        for (const Robot &robot : robots) {
            int x = (robot.x + steps * robot.dx) % mwidth;
            if (x < 0) {
                x += mwidth;
            }
            int y = (robot.y + steps * robot.dy) % mheight;
            if (y < 0) {
                y += mheight;
            }
            if (x != mwidth / 2 && y != mheight / 2) {
                int quadrant = (x > mwidth / 2) + 2 * (y > mheight / 2);
                ++quadrants[quadrant];
            }
            if (isExample()) {
                std::cerr << "{" << robot.x << "," << robot.y << "} --> {" << x << "," << y << "}\n";
            }
        }
        return std::accumulate(quadrants.begin(), quadrants.end(), 1, std::multiplies{});
    }
public:
    Day14 (const std::string &input) : aoc2024::Impl(input) {
        std::regex pattern {R"(p=([0-9]+),([0-9]+) v=(-?[0-9]+),(-?[0-9]+))"};
        for (std::smatch const &match : std::ranges::subrange(std::sregex_iterator(input.begin(), input.end(), pattern), std::sregex_iterator())) {
            robots.emplace_back(
                std::stoll(match[1]),
                std::stoll(match[2]),
                std::stoll(match[3]),
                std::stoll(match[4])
            );
        }
    }

    void part1 () final {
        std::cout << safetyFactor(100) << std::endl;
    }

    void part2 () final {
        if (isExample()) {
            std::cout << "example is not valid\n";
            return;
        }
        std::array<std::array<bool, width>, height> grid;
        std::array<int, width> cols {};
        std::array<int, height> rows {};
        int xmod = -1, ymod = -1;
        constexpr unsigned mindim = std::min(width, height);
        for (int steps = 0; steps < mindim; ++steps) {
            std::ranges::for_each(grid, [](auto &row) {row.fill(false);});
            cols.fill(0);
            rows.fill(0);
            for (const Robot &robot : robots) {
                int x = (robot.x + steps * robot.dx) % static_cast<int>(width);
                if (x < 0) {
                    x += width;
                }
                int y = (robot.y + steps * robot.dy) % static_cast<int>(height);
                if (y < 0) {
                    y += height;
                }
                cols[x] += !grid[y][x];
                rows[y] += !grid[y][x];
                grid[y][x] = true;
            }
            if (xmod == -1 && std::ranges::count_if(cols, [](int c) {return c > 10;}) > 5) {
                xmod = steps;
            }
            if (ymod == -1 && std::ranges::count_if(rows, [](int c) {return c > 10;}) > 5) {
                ymod = steps;
            }
        }
        for (int i = 0; i < height; ++i) {
            if ((xmod + i * width) % height == ymod) {
                std::cout << xmod + i * width << std::endl;
                break;
            }
        }
    }
};

int main () {
    aoc2024::main<Day14>();
}

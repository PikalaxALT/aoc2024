#include <ranges>
#include <array>
#include <set>
#include "aoc2024.hpp"

class Day12 : public aoc2024::Impl {
    std::vector<std::string> plots;
    size_t width;
    size_t height;

    static constexpr std::array<std::array<int, 2>, 4> directions {{
        {{ -1,  0 }},
        {{  0,  1 }},
        {{  1,  0 }},
        {{  0, -1 }},
    }};
    std::set<std::pair<size_t, size_t>> visited;
    std::set<std::pair<size_t, size_t>> getRegion(ssize_t x, ssize_t y, char c) {
        std::set<std::pair<size_t, size_t>> result;
        if (
                y >= 0
                && y < height
                && x >= 0
                && x < width
                && plots[y][x] == c
                && visited.insert({y, x}).second
            ) {
            result.insert({y, x});
            for (auto [dy, dx] : directions) {
                result.insert_range(getRegion(x + dx, y + dy, c));
            }
        }
        return result;
    }
    size_t getPerimeter(std::set<std::pair<size_t, size_t>> const& region, char c) const {
        size_t result = 0;
        std::set<std::tuple<ssize_t, ssize_t, int>> edgeSpecs;
        for (auto [y, x] : region) {
            for (int i = 0; i < 4; ++i) {
                auto [dy, dx] = directions[i];
                ssize_t ny = y + dy;
                ssize_t nx = x + dx;
                if (ny < 0 || ny >= height || nx < 0 || nx >= width || plots[ny][nx] != c) {
                    ++result;
                }
            }
        }
        return result;
    }
    size_t getNumSides(std::set<std::pair<size_t, size_t>> const& region, char c) const {
        std::set<std::tuple<ssize_t, ssize_t, int>> edgeSpecs;
        for (auto [y, x] : region) {
            for (int i = 0; i < 4; ++i) {
                auto [dy, dx] = directions[i];
                ssize_t ny = y + dy;
                ssize_t nx = x + dx;
                if (ny < 0 || ny >= height || nx < 0 || nx >= width || plots[ny][nx] != c) {
                    edgeSpecs.emplace(y, x, i);
                }
            }
        }
        size_t result = 0;
        while (!edgeSpecs.empty()) {
            ++result;
            auto [y, x, i] = *edgeSpecs.begin();
            edgeSpecs.erase(edgeSpecs.begin());
            int dy, dx;
            if (i % 2) {
                dx = 0;
                dy = 1;
            } else {
                dx = 1;
                dy = 0;
            }
            for (int j = 1; ; ++j) {
                auto it = edgeSpecs.find(std::tuple<ssize_t, ssize_t, int>(y + j * dy, x + j * dx, i));
                if (it == edgeSpecs.end()) {
                    break;
                }
                edgeSpecs.erase(it);
            }
            dy = -dy;
            dx = -dx;
            for (int j = 1; ; ++j) {
                auto it = edgeSpecs.find(std::tuple<ssize_t, ssize_t, int>(y + j * dy, x + j * dx, i));
                if (it == edgeSpecs.end()) {
                    break;
                }
                edgeSpecs.erase(it);
            }
        }
        return result;
    }
public:
    Day12 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        plots = std::string_view(input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::views::transform([](const auto &x) { return std::string(x); })
            | std::ranges::to<std::vector>();
        if (input.empty()) {
            height = width = 0;
            return;
        }
        height = plots.size();
        width = plots[0].size();
    }

    void part1 () final {
        int price = 0;
        visited.clear();
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                auto c = plots[i][j];
                auto region = getRegion(j, i, c);
                if (!region.empty()) {
                    auto area = region.size();
                    auto perimeter = getPerimeter(region, c);
                    price += area * perimeter;
                }
            }
        }
        std::cout << price << std::endl;
    }

    void part2 () final {
        int price = 0;
        visited.clear();
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                auto c = plots[i][j];
                auto region = getRegion(j, i, c);
                if (!region.empty()) {
                    auto area = region.size();
                    auto sides = getNumSides(region, c);
                    price += area * sides;
                }
            }
        }
        std::cout << price << std::endl;
    }
};

int main () {
    aoc2024::main<Day12>();
}

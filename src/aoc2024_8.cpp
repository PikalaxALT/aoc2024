#include <map>
#include <set>
#include <ranges>
#include "aoc2024.hpp"

class Day8 : public aoc2024::Impl {
    std::map<char, std::vector<std::pair<size_t, size_t>>> antennas;
    std::vector<std::string> grid;
    size_t height;
    size_t width;
public:
    Day8 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        grid = std::string_view(input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::views::transform([](const auto &x) { return std::string(x); })
            | std::ranges::to<std::vector>();
        height = grid.size();
        width = grid.empty() ? 0 : grid[0].size();
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                if (grid[i][j] != '.') {
                    antennas[grid[i][j]].emplace_back(i, j);
                }
            }
        }
    }

    void part1 () final {
        std::set<std::pair<ssize_t, ssize_t>> antinodes;
        for (const auto &[key, nodes] : antennas) {
            for (size_t a = 1; a < nodes.size(); ++a) {
                for (size_t b = 0; b < a; ++b) {
                    std::pair<size_t, size_t> vec_a_to_b { nodes[b].first - nodes[a].first, nodes[b].second - nodes[a].second };
                    antinodes.emplace(nodes[a].first - vec_a_to_b.first, nodes[a].second - vec_a_to_b.second);
                    antinodes.emplace(nodes[b].first + vec_a_to_b.first, nodes[b].second + vec_a_to_b.second);
                }
            }
        }
        size_t count = (std::views::filter(antinodes, [this](const auto &coord) {
            return coord.first >= 0 && coord.first < height && coord.second >= 0 && coord.second < width;
        }) | std::ranges::to<std::set>()).size();
        std::cout << count << std::endl;
    }

    void part2 () final {
        std::set<std::pair<ssize_t, ssize_t>> antinodes;
        for (const auto &[key, nodes] : antennas) {
            antinodes.insert(nodes.begin(), nodes.end());
            for (size_t a = 1; a < nodes.size(); ++a) {
                for (size_t b = 0; b < a; ++b) {
                    std::pair<size_t, size_t> vec_a_to_b { nodes[b].first - nodes[a].first, nodes[b].second - nodes[a].second };

                    std::pair<ssize_t, ssize_t> antinode {nodes[a].first - vec_a_to_b.first, nodes[a].second - vec_a_to_b.second};
                    while (antinode.first >= 0 && antinode.first < height && antinode.second >= 0 && antinode.second < width) {
                        antinodes.insert(antinode);
                        antinode.first -= vec_a_to_b.first;
                        antinode.second -= vec_a_to_b.second;
                    }
                    antinode.first = nodes[b].first + vec_a_to_b.first;
                    antinode.second = nodes[b].second + vec_a_to_b.second;
                    while (antinode.first >= 0 && antinode.first < height && antinode.second >= 0 && antinode.second < width) {
                        antinodes.insert(antinode);
                        antinode.first += vec_a_to_b.first;
                        antinode.second += vec_a_to_b.second;
                    }
                }
            }
        }
        size_t count = antinodes.size();
        std::cout << count << std::endl;
    }
};

int main () {
    aoc2024::main<Day8>();
}

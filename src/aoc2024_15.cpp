#include <ranges>
#include <set>
#include "aoc2024.hpp"

class Day15 : public aoc2024::Impl {
    static constexpr std::array<std::array<int, 2>, 4> directions {{
        {{  0, -1 }}, // north
        {{  1,  0 }}, // east
        {{  0,  1 }}, // south
        {{ -1,  0 }}, // west
    }};
    static constexpr std::string direction_chars {"^>v<"};
    size_t width;
    size_t height;
    std::set<std::array<int, 2>> boxes;
    std::set<std::array<int, 2>> walls;
    std::array<int, 2> robot {};
    std::string moves;
    std::vector<std::vector<char>> room;
    std::string m_input;
public:
    Day15 (const std::string &input) : aoc2024::Impl(input), m_input(input) {
        init();
    }

    void reset() {
        boxes.clear();
        walls.clear();
        robot.fill(0);
        room.clear();
        moves.clear();
        width = height = 0;
        init();
    }

    void init() {
        using std::operator""sv;
        auto v = std::string_view(m_input)
            | std::views::split("\n"sv)
            | std::views::transform([](const auto &x) { return std::string_view(x); })
            | std::ranges::to<std::vector>();
        auto it = v.begin();
        if (it == v.end()) {
            return;
        }
        width = it->size();
        for (int y = 0; it != v.end(); ++y, ++it) {
            if (it->empty()) {
                height = y;
                break;
            }
            room.push_back(std::ranges::to<std::vector>(*it));
            for (int x = 0; auto c : *it) {
                switch (c) {
                case '#':
                    walls.insert({x, y});
                    break;
                case 'O':
                    boxes.insert({x, y});
                    break;
                case '@':
                    robot = {x, y};
                    break;
                }
                ++x;
            }
        }
        if (it == v.end()) {
            throw std::runtime_error("parse error");
        }
        ++it;
        while (it != v.end()) {
            moves += *it++;
        }
    }

    bool move(char c) {
        auto e = direction_chars.find(c);
        if (e == std::string::npos) {
            return false;
        }
        auto [dx, dy] = directions[e];
        auto x = robot[0] + dx;
        auto y = robot[1] + dy;
        if (walls.contains({x, y})) {
            return false;
        }
        auto box_it = boxes.find({x, y});
        if (box_it != boxes.end()) {
            do {
                x += dx;
                y += dy;
                if (walls.contains({x, y})) {
                    return false;
                }
            } while (boxes.contains({x, y}));
            boxes.erase(box_it);
            boxes.insert({x, y});
        }
        robot[0] += dx;
        robot[1] += dy;
        return true;
    }

    bool move2(char c) {
        auto e = direction_chars.find(c);
        if (e == std::string::npos) {
            std::cerr << "illegal direction: " << c << "\n";
            return false;
        }
        auto [dx, dy] = directions[e];
        auto x = robot[0] + dx;
        auto y = robot[1] + dy;
        if (walls.contains({x, y})) {
            return false;
        }
        switch (c) {
        case '<': // left
            --x;
            if (boxes.contains({x, y})) {
                do {
                    x -= 2;
                } while (boxes.contains({x, y}));
                if (walls.contains({x + 1, y})) {
                    return false;
                }
                x += 2;
                while (x < robot[0]) {
                    auto box_it = boxes.find({x, y});
                    boxes.erase(box_it);
                    boxes.insert({x - 1, y});
                    x += 2;
                }
            }
            break;
        case '>': // right
            if (boxes.contains({x, y})) {
                do {
                    x += 2;
                } while (boxes.contains({x, y}));
                if (walls.contains({x, y})) {
                    return false;
                }
                x -= 2;
                while (x > robot[0]) {
                    auto box_it = boxes.find({x, y});
                    boxes.erase(box_it);
                    boxes.insert({x + 1, y});
                    x -= 2;
                }
            }
            break;
        case '^':
        case 'v':
            std::vector<std::array<int, 2>> to_move;
            if (boxes.contains({x, y})) {
                to_move.push_back({{x, y}});
            } else if (boxes.contains({x - 1, y})) {
                to_move.push_back({{x - 1, y}});
            } else {
                break;
            }

            for (size_t i = 0; i < to_move.size(); ++i) {
                auto [cx, cy] = to_move[i];
                if (walls.contains({cx, cy + dy}) || walls.contains({cx + 1, cy + dy})) {
                    return false;
                }
                if (boxes.contains({cx, cy + dy})) {
                    to_move.push_back({{cx, cy + dy}});
                }
                if (boxes.contains({cx - 1, cy + dy})) {
                    to_move.push_back({{cx - 1, cy + dy}});
                }
                if (boxes.contains({cx + 1, cy + dy})) {
                    to_move.push_back({{cx + 1, cy + dy}});
                }
            }
            for (auto i : to_move) {
                boxes.erase(i);
            }
            for (auto i : to_move) {
                auto [cx, cy] = i;
                boxes.insert({cx, cy + dy});
            }
            break;
        }
        robot[0] += dx;
        robot[1] += dy;
        return true;
    }

    void print_layout() {
        std::ranges::for_each(room, [this](auto &row) {
            row.assign(width, '.');
        });
        for (auto [x, y] : boxes) {
            room[y][x] = 'O';
        }
        for (auto [x, y] : walls) {
            room[y][x] = '#';
        }
        room[robot[1]][robot[0]] = '@';
        std::ranges::for_each(room, [](const auto &row) {
            std::ranges::for_each(row, [](auto c) {
                std::cerr << c;
            });
            std::cerr << '\n';
        });
    }

    void print_layout_wide() {
        bool invalid = false;
        std::ranges::for_each(room, [this](auto &row) {
            row.assign(width * 2, '.');
        });
        for (auto [x, y] : boxes) {
            invalid |= room[y][x] != '.';
            invalid |= room[y][x + 1] != '.';
            room[y][x] = '[';
            room[y][x + 1] = ']';
        }
        for (auto [x, y] : walls) {
            invalid |= room[y][x] != '.';
            room[y][x] = '#';
        }
        invalid |= room[robot[1]][robot[0]] != '.';
        room[robot[1]][robot[0]] = '@';
        std::ranges::for_each(room, [](const auto &row) {
            std::ranges::for_each(row, [](auto c) {
                std::cerr << c;
            });
            std::cerr << '\n';
        });
        if (invalid) {
            std::cerr << "invalid condition! aborting\n";
            std::exit(1);
        }
    }

    void part1 () final {
        if (isExample()) {
            std::cerr << "Initial state\n";
            print_layout();
        }
        for (int i = 0; auto c : moves) {
            move(c);
            if (isExample()) {
                std::cerr << "Move " << i << " (" << c << "):\n";
                print_layout();
            }
            ++i;
        }
        int result = 0;
        for (auto [x, y] : boxes) {
            result += x + 100 * y;
        }
        std::cout << result << std::endl;
    }

    void part2 () final {
        reset();
        {
            typeof(boxes) boxes_c {};
            typeof(walls) walls_c {};
            robot[0] *= 2;
            for (auto [x, y] : boxes) {
                boxes_c.insert({x * 2, y});
            }
            for (auto [x, y] :  walls) {
                walls_c.insert({x * 2, y});
                walls_c.insert({x * 2 + 1, y});
            }
            boxes = boxes_c;
            walls = walls_c;
        }
        size_t nboxes = boxes.size();
        if (isExample()) {
            std::cerr << "Initial state:\n";
            print_layout_wide();
        }
        for (int i = 0; auto c : moves) {
            move2(c);
            if (isExample()) {
                std::cerr << "Move " << i << " (" << c << "):\n";
                print_layout_wide();
            }
            ++i;
        }
        if (nboxes != boxes.size()) {
            std::cerr << "lost or gained boxes\n";
            std::exit(1);
        }
        int result = 0;
        for (auto [x, y] : boxes) {
            result += x + 100 * y;
        }
        std::cout << result << std::endl;
    }
};

int main () {
    aoc2024::main<Day15>();
}

#include <ranges>
#include <map>
#include <numeric>
#include <charconv>
#include "aoc2024.hpp"

class Day21 : public aoc2024::Impl {
    const std::map<char, std::pair<int, int>> numpadCoords {
        {'7', {0, 0}},
        {'8', {0, 1}},
        {'9', {0, 2}},
        {'4', {1, 0}},
        {'5', {1, 1}},
        {'6', {1, 2}},
        {'1', {2, 0}},
        {'2', {2, 1}},
        {'3', {2, 2}},
        {' ', {3, 0}},
        {'0', {3, 1}},
        {'A', {3, 2}},
    };
    const std::map<char, std::pair<int, int>> dpadCoords {
        {' ', {0, 0}},
        {'^', {0, 1}},
        {'A', {0, 2}},
        {'<', {1, 0}},
        {'v', {1, 1}},
        {'>', {1, 2}},
    };
    std::vector<std::string_view> codes;
    std::array<std::string, 4> numpadGrid {{
        "789",
        "456",
        "123",
        " 0A",
    }};
    std::array<std::string, 2> dpadGrid {{
        " ^A",
        "<v>",
    }};
    static std::map<std::pair<char, char>, unsigned long long> routes;
    static int numDpads;
    static constexpr std::string numpadChars {"0123456789A"};
    static constexpr std::string dpadChars {"^v<>A"};

    std::string getPath(const std::string path, std::map<std::pair<char, char>, std::string> const& pathMap) {
        using std::operator""s;
        char pc = 'A';
        return std::accumulate(path.begin(), path.end(), ""s, [&](const std::string &a, char c) {
            std::string const& r = pathMap.at({pc, c});
            pc = c;
            return a + r;
        });
    }

    std::vector<std::string> getRobotRoute(char start, char end, std::map<char, std::pair<int, int>> const& coordsMap) {
        if (start == end) {
            return {"A"};
        }
        std::pair<int, int> const& start_pos = coordsMap.at(start);
        std::pair<int, int> const& end_pos = coordsMap.at(end);
        std::pair<int, int> const& banPos = coordsMap.at(' ');
        int dx = end_pos.second - start_pos.second;
        int dy = end_pos.first - start_pos.first;
        char lr = dx < 0 ? '<' : '>';
        char ud = dy < 0 ? '^' : 'v';
        unsigned adx = std::abs(dx);
        unsigned ady = std::abs(dy);
        std::vector<std::string> ret;
        std::string perm;
        perm.reserve(adx + ady);
        perm.append(adx, lr);
        perm.append(ady, ud);
        do {
            size_t prefix = perm.find_first_not_of(perm[0]);
            if (prefix == std::string::npos) {
                prefix = perm.length();
            }
            if (start_pos.second == banPos.second && end_pos.first == banPos.first && perm[0] == ud && prefix == ady) {
                continue;
            }
            if (start_pos.first == banPos.first && end_pos.second == banPos.second && perm[0] == lr && prefix == adx) {
                continue;
            }
            ret.emplace_back(perm + "A");
        } while (std::ranges::next_permutation(perm).found);
        return ret;
    }

    void build(int numDpads = 2) {
        using std::operator""s;
        if (numDpads == this->numDpads) {
            return;
        }
        this->numDpads = numDpads;
        routes.clear();

        std::map<std::pair<char, char>, std::string> numpadRoutes;
        std::map<std::pair<char, char>, std::vector<std::string>> dpadRoutes;

        // Get the route pairs
        for (char origin : dpadChars) {
            for (char dest : dpadChars) {
                dpadRoutes[{origin, dest}] = getRobotRoute(origin, dest, dpadCoords);
            }
        }
        aoc2024::cached<unsigned long long, char, char, int> getSegmentLength {[&](char o, char d, int n) -> unsigned long long {
            std::vector<std::string> const& rtes = dpadRoutes.at({o, d});
            if (n == 1) {
                return rtes.front().size();
            }
            return std::ranges::min(
                std::views::transform(
                    rtes,
                    [&](const std::string &r) -> unsigned long long {
                        char p = 'A';
                        return std::accumulate(r.begin(), r.end(), 0ull, [&](unsigned long long a, char c) -> unsigned long long {
                            unsigned long long x = a + getSegmentLength(p, c, n - 1);
                            p = c;
                            return x;
                        });
                    }
                )
            );
        }};

        for (char origin : numpadChars) {
            for (char dest : numpadChars) {
                std::vector<std::string> rtes = getRobotRoute(origin, dest, numpadCoords);
                routes[{origin, dest}] = std::ranges::min(
                    std::views::transform(
                        rtes,
                        [&](const std::string &rte) {
                            char prev = 'A';
                            return std::accumulate(rte.begin(), rte.end(), 0ull, [&](unsigned long long acc, char curr) -> unsigned long long {
                                unsigned long long res = acc + getSegmentLength(prev, curr, numDpads);
                                prev = curr;
                                return res;
                            });
                        }
                    )
                );
            }
        }
    }

    unsigned long long get_route_length(const std::string_view &code) {
        using std::operator""s;
        char prev = 'A';
        return std::accumulate(code.begin(), code.end(), 0ull, [&](unsigned long long a, char c) {
            unsigned long long ret = a + routes[{prev, c}];
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

    void puzzle(int numDpads) {
        unsigned long long comp = 0ull;
        build(numDpads);
        for (const auto &code : codes) {
            // get value
            unsigned long long value;
            std::from_chars(code.begin(), code.end() - 1, value);

            // get commands
            auto ans = get_route_length(code);
            std::cerr << code << ": " << ans << std::endl;

            // final complexity
            comp += ans * value;
        }
        std::cout << comp << std::endl;
    }

    void part1 () final {
        puzzle(2);
    }

    void part2 () final {
        puzzle(25);
    }
};

int main () {
    aoc2024::main<Day21>();
}

std::map<std::pair<char, char>, unsigned long long> Day21::routes {};
int Day21::numDpads = 0;

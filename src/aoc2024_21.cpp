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

    class RouteIterator {
        std::string permutation;
        bool isEnd;
        std::pair<int, int> start_pos;
        std::pair<int, int> end_pos;
        std::pair<int, int> banPos;
        int dx;
        int dy;
        char lr;
        char ud;
        bool validPermutation() {
            if (isEnd) {
                return true;
            }
            size_t turn = permutation.find_first_not_of(permutation[0]);
            if (turn == std::string::npos) {
                turn = permutation.size();
            }
            if (start_pos.first == banPos.first && end_pos.second == banPos.second && permutation[0] == lr && turn == std::abs(dx)) {
                return false;
            }
            if (start_pos.second == banPos.second && end_pos.first == banPos.first && permutation[0] == ud && turn == std::abs(dy)) {
                return false;
            }
            return true;
        }
    public:
        using difference_type = ssize_t;
        using value_type = std::string;
        RouteIterator(char start, char end, std::map<char, std::pair<int, int>> const& coordsMap) : isEnd(false) {
            if (start == end) {
                return;
            }
            start_pos = coordsMap.at(start);
            end_pos = coordsMap.at(end);
            banPos = coordsMap.at(' ');
            dx = end_pos.second - start_pos.second;
            dy = end_pos.first - start_pos.first;
            lr = dx < 0 ? '<' : '>';
            ud = dy < 0 ? '^' : 'v';
            permutation.reserve(std::abs(dx) + std::abs(dy));
            permutation.append(std::abs(dx), lr);
            permutation.append(std::abs(dy), ud);
            if (!validPermutation()) {
                ++*this;
            }
        }
        RouteIterator() : isEnd(true) {}
        RouteIterator(const RouteIterator &rhs) = default;
        RouteIterator(RouteIterator &&rhs) = default;
        RouteIterator &operator=(const RouteIterator &rhs) = default;
        RouteIterator &operator=(RouteIterator &&rhs) = default;
        RouteIterator &operator++() {
            while (!isEnd && !(isEnd = !std::ranges::next_permutation(permutation).found) && !validPermutation()) {}
            return *this;
        }
        RouteIterator operator++(int) {
            RouteIterator ret = *this;
            ++*this;
            return ret;
        }
        bool operator==(const RouteIterator &rhs) const {
            if (isEnd) {
                return rhs.isEnd;
            }
            return !rhs.isEnd && permutation == rhs.permutation;
        }
        bool operator!=(const RouteIterator &rhs) const {
            return !(*this == rhs);
        }
        std::string operator*() const {
            if (isEnd) {
                throw std::out_of_range("Day21::RouteIterator::operator*()");
            }
            return permutation + "A";
        }
    };

    std::vector<std::string> getRobotRouteSegments(char start, char end, std::map<char, std::pair<int, int>> const& coordsMap) {
        return std::vector<std::string>(RouteIterator{start, end, coordsMap}, RouteIterator{});
    }

    aoc2024::cached<std::vector<std::string>, char, char> getNumpadRouteSegments {[this](char start, char end) -> std::vector<std::string> {
        return getRobotRouteSegments(start, end, numpadCoords);
    }};

    aoc2024::cached<std::vector<std::string>, char, char> getDpadRouteSegments {[this](char start, char end) -> std::vector<std::string> {
        return getRobotRouteSegments(start, end, dpadCoords);
    }};

    aoc2024::cached<std::string, char, char> getBestDpad2RouteSegment {[this](char start, char end) -> std::string {
        return *RouteIterator{start, end, dpadCoords};
    }};

    std::string getBestDpad2Route(std::string path) {
        std::string cur;
        char prev = 'A';
        for (char c : path) {
            cur += getBestDpad2RouteSegment(prev, c);
            prev = c;
        }
        return cur;
    }

    std::string getBestDpad1RouteSegment(std::vector<std::string> paths, int numDpads) {
        std::string best;
        for (const auto &path : paths) {
            auto cur = getBestDpad1Route(path, numDpads);
            if (best.empty() || best.size() > cur.size()) {
                best = cur;
            }
        }
        return best;
    }

    std::string getBestDpad1Route(std::string const& path, int numDpads) {
        static std::map<std::tuple<std::string, int>, std::string> cache;
        auto cit = cache.find({path, numDpads});
        if (cit != cache.end()) {
            return cit->second;
        }
        auto &cur = cache[{path, numDpads}];
        char prev = 'A';
        for (char c : path) {
            if (numDpads <= 1) {
                cur += getBestDpad2RouteSegment(prev, c);
            } else {
                cur += getBestDpad1RouteSegment(getDpadRouteSegments(prev, c), numDpads - 1);
            }
            prev = c;
        }
        return cur;
    }

    std::string getBestNumpadRouteSegment (char start, char end, int numDpads) {
        using std::operator""s;

        std::vector<std::string> numpadRoutes = getNumpadRouteSegments(start, end);
        std::string ret;
        for (const auto &rte : numpadRoutes) {
            auto cur = getBestDpad1Route(rte, numDpads);
            if (ret.empty() || ret.size() > cur.size()) {
                ret = cur;
            }
        }
        return ret;
    }

    std::string get_route(const std::string_view &code, int numDpads = 2) {
        using std::operator""s;
        char prev = 'A';
        return std::accumulate(code.begin(), code.end(), ""s, [&](const std::string &a, char c) {
            std::string ret = a + getBestNumpadRouteSegment(prev, c, numDpads);
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
        unsigned long long comp = 0ull;
        for (const auto &code : codes) {
            // get value
            unsigned long long value;
            std::from_chars(code.begin(), code.end() - 1, value);

            // get commands
            auto ans = get_route(code, 2);

            // final complexity
            comp += ans.size() * value;
        }
        std::cout << comp << std::endl;
    }

    void part2 () final {
        unsigned long long comp = 0ull;
        for (const auto &code : codes) {
            // get value
            unsigned long long value;
            std::from_chars(code.begin(), code.end() - 1, value);

            // get commands
            auto ans = get_route(code, 25);

            // final complexity
            comp += ans.size() * value;
        }
        std::cout << comp << std::endl;
    }
};

int main () {
    aoc2024::main<Day21>();
}

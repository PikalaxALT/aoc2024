#include <ranges>
#include <set>
#include "aoc2024.hpp"

class Day23 : public aoc2024::Impl {
    std::map<std::string, std::set<std::string>> lanLinks;
public:
    Day23 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        for (auto line : std::string_view(input) | std::views::split("\n"sv)) {
            auto spl = std::views::split(line, "-"sv)
                | std::views::transform([](const auto &x) { return std::string(x.begin(), x.end()); })
                | std::ranges::to<std::vector>();
            lanLinks[spl[0]].insert(spl[1]);
            lanLinks[spl[1]].insert(spl[0]);
        }
    }

    void part1 () final {
        int answer = 0;
        std::set<std::array<std::string, 3>> seen;
        for (auto [c1, s] : lanLinks) {
            for (auto it1 = s.begin(); it1 != s.end(); ++it1) {
                for (auto it2 = std::next(it1); it2 != s.end(); ++it2) {
                    std::array<std::string, 3> group {c1, *it1, *it2};
                    std::ranges::sort(group);
                    if (seen.insert(group).second && lanLinks[*it1].contains(*it2) && std::ranges::any_of(group, [](const std::string &s) {
                        return s.starts_with('t');
                    })) {
                        ++answer;
                    }
                }
            }
        }
        std::cout << answer << std::endl;
    }

    void part2 () final {
        // Collect all pairs;
        std::set<std::set<std::string>> pairs;
        for (auto [c1, s] : lanLinks) {
            for (auto c2 : s) {
                pairs.insert({c1, c2});
            }
        }
        std::set<std::set<std::string>> groups;
        do {
            // this is incredibly slow but i'm too smoothbrained to implement a faster solution
            groups.clear();
            for (auto clique : pairs) {
                for (auto [c1, s] : lanLinks) {
                    if (clique.contains(c1)) {
                        continue;
                    }
                    if (std::ranges::all_of(clique, [&](const std::string &c2) {
                        return s.contains(c2);
                    })) {
                        auto it = clique.insert(c1).first;
                        groups.insert(clique);
                        clique.erase(it);
                    }
                }
            }
            std::cerr << "looking for cliques of size " << groups.begin()->size() << ": found " << groups.size() << std::endl;
            pairs = groups;
        } while (groups.size() > 1);
        if (groups.empty()) {
            throw std::runtime_error("largest clique is not unique");
        }
        aoc2024::print(std::cout, *groups.begin(), ",") << std::endl;
    }
};

int main () {
    aoc2024::main<Day23>();
}

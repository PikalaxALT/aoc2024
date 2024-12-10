#include <algorithm>
#include <ranges>
#include "aoc2024.hpp"

class Day9 : public aoc2024::Impl {
    std::string m_input;
public:
    Day9 (const std::string &input) : aoc2024::Impl(input), m_input(input) {}

    void part1 () final {
        std::vector<int> blocks;
        {
        auto x = m_input | std::views::transform([](char c) { return c - '0'; });
        size_t len = std::ranges::fold_left(x, 0, std::plus{});
        blocks.reserve(len);
        }
        bool is_file = true;
        int file_num = 0;
        for (auto c : m_input) {
            if (is_file) {
                blocks.insert(blocks.end(), c - '0', file_num);
                ++file_num;
            } else {
                blocks.insert(blocks.end(), c - '0', -1);
            }
            is_file = !is_file;
        }
        auto i_free = std::find(blocks.begin(), blocks.end(), -1);
        auto i_used = std::find_if_not(blocks.rbegin(), blocks.rend(), [](int x) { return x == -1; });
        while ((blocks.rend() - i_used) > (i_free - blocks.begin())) {
            std::swap(*i_used, *i_free);
            i_used = std::find_if_not(i_used, blocks.rend(), [](int x) { return x == -1; });
            i_free = std::find(i_free, blocks.end(), -1);
        }
        // for (int x : blocks) {
        //     std::cerr << static_cast<char>(x == -1 ? '.' : x + '0');
        // }
        // std::cerr << '\n';
        unsigned long long total = 0;
        for (int i = 0; i < blocks.size(); ++i) {
            if (blocks[i] == -1) {
                break;
            }
            total += i * blocks[i];
        }
        std::cout << total << std::endl;
    }

    void part2 () final {
        std::vector<int> blocks;
        {
        auto x = m_input | std::views::transform([](char c) { return c - '0'; });
        size_t len = std::ranges::fold_left(x, 0, std::plus{});
        blocks.reserve(len);
        }
        bool is_file = true;
        int file_num = 0;
        for (auto c : m_input) {
            if (is_file) {
                blocks.insert(blocks.end(), c - '0', file_num);
                ++file_num;
            } else {
                blocks.insert(blocks.end(), c - '0', -1);
            }
            is_file = !is_file;
        }
        // for (int x : blocks) {
        //     std::cerr << (x == -1 ? "." : std::format("[{}]", x));
        // }
        // std::cerr << '\n';
        auto i_used = std::find_if_not(blocks.rbegin(), blocks.rend(), [](int x) { return x == -1; });
        while (i_used < blocks.rend()) {
            size_t fileSize = std::find_if_not(i_used, blocks.rend(), [=](int x) { return x == *i_used;}) - i_used;
            auto i_free = std::find(blocks.begin(), blocks.end(), -1);
            size_t freeSize;
            while (i_free < blocks.end() && (i_free - blocks.begin()) < (blocks.rend() - i_used - fileSize)) {
                freeSize = std::find_if_not(i_free, blocks.end(), [=](int x) { return x == -1; }) - i_free;
                if (freeSize >= fileSize) {
                    for (int i = 0; i < fileSize; ++i) {
                        std::swap(i_used[i], i_free[i]);
                    }
                    break;
                }
                i_free = std::find(i_free + freeSize, blocks.end(), -1);
            }
            i_used = std::find_if_not(i_used + fileSize, blocks.rend(), [](int x) { return x == -1; });
            // for (int x : blocks) {
            //     std::cerr << (x == -1 ? "." : std::format("[{}]", x));
            // }
            // std::cerr << '\n';
        }
        unsigned long long total = 0;
        for (int i = 0; i < blocks.size(); ++i) {
            if (blocks[i] != -1) {
                total += i * blocks[i];
            }
        }
        std::cout << total << std::endl;
    }
};

int main () {
    aoc2024::main<Day9>();
}

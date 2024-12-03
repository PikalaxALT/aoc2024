#include <algorithm>
#include "aoc2024.hpp"

class Day3 : public aoc2024::Impl {
    std::string m_input;
public:
    Day3 (const std::string &input) : aoc2024::Impl(input), m_input(input) {}

    void part1 () final {
        size_t pos = 0;
        size_t slen = m_input.length();
        int total = 0;
        while ((pos = m_input.find("mul", pos)) != std::string::npos) {
            pos += 3;
            if (m_input[pos] != '(') {
                continue;
            }
            ++pos;
            if (pos >= slen) {
                break;
            }
            size_t npos = m_input.find_first_not_of("0123456789", pos);
            if (npos == pos || npos == std::string::npos || m_input[npos] != ',') {
                continue;
            }
            int a = std::stoi(m_input.substr(pos, npos - pos));
            pos = npos + 1;
            if (pos >= slen) {
                break;
            }
            npos = m_input.find_first_not_of("0123456789", pos);
            if (npos == pos || npos == std::string::npos || m_input[npos] != ')') {
                continue;
            }
            int b = std::stoi(m_input.substr(pos, npos - pos));
            pos = npos + 1;
            total += a * b;
        }
        std::cout << total << std::endl;
    }

    void part2 () final {
        size_t pos = 0;
        size_t slen = m_input.length();
        long long total = 0;
        bool enabled = true;
        while (pos < slen) {
            std::array<size_t, 3> search {
                m_input.find("mul(", pos),
                m_input.find("don't()", pos),
                m_input.find("do()", pos)
            };
            pos = *std::ranges::min_element(search);
            if (pos == std::string::npos) {
                break;
            }
            if (pos == search[0]) {
                pos += 4;
                long long a, b;
                size_t npos = m_input.find_first_not_of("0123456789", pos);
                if (npos == pos || npos == std::string::npos || m_input[npos] != ',') {
                    continue;
                }
                a = std::stoll(m_input.substr(pos, npos - pos));
                pos = npos + 1;
                npos = m_input.find_first_not_of("0123456789", pos);
                if (npos == pos || npos == std::string::npos || m_input[npos] != ')') {
                    continue;
                }
                b = std::stoll(m_input.substr(pos, npos - pos));
                pos = npos + 1;
                if (enabled) {
                    total += a * b;
                }
            } else if (pos == search[1]) {
                enabled = false;
                pos += 7;
            } else if (pos == search[2]) {
                enabled = true;
                pos += 4;
            } else {
                break;
            }
        }
        std::cout << total << std::endl;
    }
};

int main() {
    aoc2024::main<Day3>();
}

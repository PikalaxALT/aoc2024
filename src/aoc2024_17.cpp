#include <ranges>
#include <charconv>
#include <deque>
#include "aoc2024.hpp"

struct end_of_program : public std::exception {};
struct invalid_operand : public std::exception {};

class Day17 : public aoc2024::Impl {
    // cpu state
    std::vector<unsigned> program;
    unsigned pc = 0;
    std::array<unsigned long long, 3> registers;
    std::array<unsigned long long, 3> registers_init;
    static constexpr std::array<std::string, 3> registerNames {{
        "A",
        "B",
        "C",
    }};
    static constexpr int A = 0;
    static constexpr int B = 1;
    static constexpr int C = 2;
public:
    Day17 (const std::string &input) : aoc2024::Impl(input) {
        using std::operator""sv;
        size_t pos = 0;
        size_t npos = 0;
        // get register A
        pos = input.find_first_of("0123456789", npos);
        if (pos == std::string::npos) {
            std::cerr << "nothing\n";
            return;
        }
        npos = input.find_first_not_of("0123456789", pos);
        registers_init[A] = std::stoull(input.substr(pos, npos - pos));
        // get register B
        pos = input.find_first_of("0123456789", npos);
        npos = input.find_first_not_of("0123456789", pos);
        registers_init[B] = std::stoull(input.substr(pos, npos - pos));
        // get register C
        pos = input.find_first_of("0123456789", npos);
        npos = input.find_first_not_of("0123456789", pos);
        registers_init[C] = std::stoull(input.substr(pos, npos - pos));
        // get program
        pos = input.find_first_of("0123456789", npos);
        program = std::string_view(input.substr(pos))
            | std::views::split(","sv)
            | std::views::transform([](const auto &x) {
                unsigned v;
                std::from_chars(x.begin(), x.end(), v);
                return v;
            })
            | std::ranges::to<std::vector>();
    }

    unsigned read() {
        if (pc >= program.size()) {
            throw end_of_program();
        }
        return program[pc++];
    }

    unsigned long long combo(unsigned ret) {
        switch (ret) {
        case 0:
        case 1:
        case 2:
        case 3:
            return ret;
        case 4:
        case 5:
        case 6:
            return registers[ret - 4];
        case 7:
        default:
            throw invalid_operand();
        }
    }

    unsigned execute() {
        unsigned long long operand;
        switch (read()) {
        case 0: // adv
            operand = combo(read());
            registers[A] = registers[A] >> operand;
            break;
        case 1: // bxl
            operand = read();
            registers[B] ^= operand;
            break;
        case 2: // bst
            operand = combo(read());
            registers[B] = operand % 8;
            break;
        case 3: // jnz
            operand = read();
            if (registers[A] != 0) {
                pc = operand;
            }
            break;
        case 4: // bxc
            operand = read();
            registers[B] ^= registers[C];
            break;
        case 5: // out
            operand = combo(read());
            return operand % 8;
        case 6: // bdv
            operand = combo(read());
            registers[B] = registers[A] >> operand;
            break;
        case 7: // cdv
            operand = combo(read());
            registers[C] = registers[A] >> operand;
            break;
        }

        return -1u;
    }

    std::vector<unsigned> run(unsigned long long override_A = -1u) {
        registers = registers_init;
        pc = 0;
        if (override_A != -1u) {
            registers[A] = override_A;
        }
        std::vector<unsigned> ret;
        unsigned value;
        try {
            while (true) {
                if ((value = execute()) != -1u) {
                    ret.push_back(value);
                }
            }
        } catch (end_of_program) {}
        return ret;
    }

    void part1 () final {
        using aoc2024::operator<<;
        std::vector<unsigned> outputs = run();
        std::cout << outputs << "\n";
    }

    std::string combo_dism(unsigned x) {
        switch (x) {
        case 0:
        case 1:
        case 2:
        case 3:
            return std::to_string(x);
        case 4:
        case 5:
        case 6:
            return registerNames[x - 4];
        }

        return "<invalid>";
    }

    void disassemble() {
        for (int i = 0; i < program.size() - 1; i += 2) {
            switch (program[i]) {
            case 0:
                std::cout << "adv " << combo_dism(program[i + 1]);
                break;
            case 1:
                std::cout << "bxl " << program[i + 1];
                break;
            case 2:
                std::cout << "bst " << combo_dism(program[i + 1]);
                break;
            case 3:
                std::cout << "jnz " << program[i + 1];
                break;
            case 4:
                std::cout << "bxc " << program[i + 1];
                break;
            case 5:
                std::cout << "out " << combo_dism(program[i + 1]);
                break;
            case 6:
                std::cout << "bdv " << combo_dism(program[i + 1]);
                break;
            case 7:
                std::cout << "cdv " << combo_dism(program[i + 1]);
                break;
            default:
                std::cout << "<invalid opcode>";
            }
            std::cout << "\t; " << program[i] << " " << program[i + 1] << "\n";
        }
    }

    void part2 () final {
        // assume the program ends in `out <reg> / jnz 0` and has a pattern of shifting A right by 3 each loop
        // my test program is:
        //  b = a % 8
        //  b ^= 3
        //  c = a >> b
        //  a >>= 3
        //  b ^= c  --> operand 3 is ignored
        //  b ^= 5
        //  out(b % 8)
        //  while a != 0
        // simplify:
        //  b = (a % 8) ^ 3
        //  c = a >> b
        //  b ^= c ^ 5

        unsigned long long result = 0;
        unsigned long shift = 0;
        for (int i = 0; i < program.size() - 1; i += 2) {
            if (program[i] == 0) {
                shift = program[i + 1];
                break;
            }
        }
        if (shift == 0 || shift >= 4) {
            throw invalid_operand();
        }
        unsigned long long limit = 8;
        bool begin = true;
        bool tried = false;
        std::deque<unsigned long long> possibles { 0ull };
        for (auto r = program.crbegin(); r != program.crend(); ++r) {
            bool found = false;
            size_t n_possibles = possibles.size();
            for (size_t k = 0; k < n_possibles; ++k) {
                result = possibles.front() << shift;
                possibles.pop_front();
                for (unsigned long long i = 0; i < limit; ++i) {
                    auto output = run(result | i);
                    if (std::ranges::equal(output, std::ranges::subrange(r.base() - 1, program.end()))) {
                        possibles.push_back(result | i);
                    }
                }
            }
            if (possibles.empty()) {
                std::cerr << "abort\n";
                return;
            }
        }
        result = std::ranges::min(possibles);
        std::cout << result << std::endl;
    }
};

int main () {
    aoc2024::main<Day17>();
}

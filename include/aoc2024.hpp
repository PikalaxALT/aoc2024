#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <boost/log/trivial.hpp>
#include <source_location>

namespace fs = std::filesystem;

namespace aoc2024 {
    constexpr int day = AOC2024_DAY;
    constexpr std::source_location here = std::source_location::current();

    static inline std::string intro_message () {
        return "===================\n"
               " AOC2024 Day " + std::to_string(day) + "\n"
               "===================";
    }

    static inline std::string read_whole_file (const fs::path &data_path) {
        if (fs::exists(data_path)) {
            BOOST_LOG_TRIVIAL(debug) << "path exists: " << data_path;
            std::ifstream file { data_path };
            std::stringstream str_contents;
            str_contents << file.rdbuf();
            return str_contents.str();
        }

        BOOST_LOG_TRIVIAL(debug) << "path not found: " << data_path;
        return {};
    }

    static inline std::string get_example () {
        const fs::path source_path { here.file_name() };
        const fs::path data_path = source_path.parent_path().parent_path() / "data" / "examples" / (std::to_string(day) + ".txt");
        return read_whole_file(data_path);
    }

    static inline std::string get_testcase () {
        const fs::path source_path { here.file_name() };
        const fs::path data_path = source_path.parent_path().parent_path() / "data" / "testcases" / (std::to_string(day) + ".txt");
        return read_whole_file(data_path);
    }

    class Impl {
    public:
        virtual void part1 (const std::string &input) {
            std::cout << "Day " << day << " part 1 not implemented\n";
        }
        virtual void part2 (const std::string &input) {
            std::cout << "Day " << day << " part 2 not implemented\n";
        }
    };

    template <typename cls = Impl>
    void main () {
        std::cout << intro_message() << "\n";
        std::string example = get_example();
        std::string testcase = get_testcase();

        if (!example.empty()) {
            std::cout << "\n === PART 1 EXAMPLE ===\n\n";
            cls kls {};
            kls.part1(example);
        }

        if (!testcase.empty()) {
            std::cout << "\n === PART 1 TEST CASE ===\n\n";
            cls kls {};
            kls.part1(testcase);
        }

        if (!example.empty()) {
            std::cout << "\n === PART 2 EXAMPLE ===\n\n";
            cls kls {};
            kls.part2(example);
        }

        if (!testcase.empty()) {
            std::cout << "\n === PART 2 TEST CASE ===\n\n";
            cls kls {};
            kls.part2(testcase);
        }
    }
}

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <map>
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
        bool example = false;
    public:
        Impl() = delete;
        Impl(const std::string &input) {}
        virtual void part1 () {
            std::cout << "Day " << day << " part 1 not implemented\n";
        }
        virtual void part2 () {
            std::cout << "Day " << day << " part 2 not implemented\n";
        }
        bool isExample() const {
            return example;
        }
        void setExample(bool ex = true) {
            example = ex;
        }
    };

    template <typename cls = Impl>
    void main () {
        std::cout << intro_message() << "\n";
        std::string example = get_example();
        std::string testcase = get_testcase();
        example = example.substr(0, example.find_last_not_of('\n') + 1);
        testcase = testcase.substr(0, testcase.find_last_not_of('\n') + 1);

        cls example_cls {example};
        example_cls.setExample();
        cls testcase_cls {testcase};

        if (!example.empty()) {
            std::cout << "\n === PART 1 EXAMPLE ===\n\n";
            example_cls.part1();
        }

        if (!testcase.empty()) {
            std::cout << "\n === PART 1 TEST CASE ===\n\n";
            testcase_cls.part1();
        }

        if (!example.empty()) {
            std::cout << "\n === PART 2 EXAMPLE ===\n\n";
            example_cls.part2();
        }

        if (!testcase.empty()) {
            std::cout << "\n === PART 2 TEST CASE ===\n\n";
            testcase_cls.part2();
        }
    }

    template <typename S, typename R>
    concept has_left_shift = requires (S s, R r) { s << r; };

    template<typename Strm, std::ranges::range Range, typename Char = char>
    Strm &print(Strm &strm, Range const &range, const Char *sep = ", ") {
        for (const Char *_sep = ""; const auto &x : range) {
            strm << _sep << x;
            _sep = sep;
        }
        return strm;
    }

    template<typename Strm, std::ranges::range Range, typename Char = char>
    Strm &print(Strm &strm, Range const &range, const std::basic_string<Char> &sep) {
        for (std::basic_string<Char> _sep = ""; const auto &x : range) {
            strm << _sep << x;
            _sep = sep;
        }
        return strm;
    }

    template<typename Strm, std::ranges::range Range, typename Char = char>
        requires (std::derived_from<Strm, std::basic_ostream<Char>> && !has_left_shift<Strm, Range>)
    Strm &operator<<(Strm &strm, Range const &range) {
        return print(strm, range);
    }

    template<typename R, typename... A>
    class cached {
        std::function<R(A...)> _fn;
        std::map<std::tuple<A...>, R> _cache;
    public:
        cached(std::function<R(A...)>&& fn) : _fn(fn) {}
        R operator()(A... a) {
            // search in cache
            auto key = std::make_tuple(std::forward<A>(a)...);
            auto rit = _cache.find(key);
            if (rit != _cache.end()) {
                return rit->second;
            }

            // cache miss, compute and return
            return _cache[key] = _fn(std::forward<A>(a)...);
        }
    };
}

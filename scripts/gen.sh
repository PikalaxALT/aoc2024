#!/usr/bin/env bash

set -euo pipefail

for i in {1..31}; do
    if ! [ -f src/aoc2024_${i}.cpp ]; then
        cat <<-EOF > src/aoc2024_${i}.cpp
#include "aoc2024.hpp"

class Day$i : public aoc2024::Impl {
public:
    Day$i (const std::string &input) : aoc2024::Impl(input) {}

    void part1 () final {}

    void part2 () final {}
};

int main () {
    aoc2024::main<Day$i>();
}
EOF
    fi
done

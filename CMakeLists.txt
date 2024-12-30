cmake_minimum_required(VERSION 3.30)
project(AOC2024 CXX)

string(TIMESTAMP DAY %d)
string(TIMESTAMP YEAR %Y)

find_package(Boost REQUIRED COMPONENTS log)

set(CMAKE_CXX_STANDARD 23)
# Temporary until clang adds support for modules
if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()
if(${YEAR} GREATER 2024 OR ${DAY} GREATER 25)
    set(DAY 25)
endif()
foreach(i RANGE 1 ${DAY})
    if(IS_READABLE ${CMAKE_CURRENT_SOURCE_DIR}/src/aoc2024_${i}.cpp)
        add_executable(aoc2024_${i} src/aoc2024_${i}.cpp)
        target_include_directories(aoc2024_${i} PRIVATE include)
        target_compile_definitions(aoc2024_${i} PRIVATE AOC2024_DAY=${i})
        target_link_libraries(aoc2024_${i} PUBLIC Boost::log)
    endif()
endforeach()

cmake_minimum_required(VERSION 2.6.0)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(aoc2023 C)

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)

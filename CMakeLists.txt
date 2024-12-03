cmake_minimum_required(VERSION 3.30)

project(advent_of_code LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

include(cmake/Day.cmake)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(2024)
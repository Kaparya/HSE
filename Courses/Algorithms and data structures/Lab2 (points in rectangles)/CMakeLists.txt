cmake_minimum_required(VERSION 3.27)
project(Lab2__points_in_rectangles_)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab2__points_in_rectangles_
        src/main.cpp
        src/Algorithms/brute_force.cpp
        src/Algorithms/map_algorithm.cpp
        src/Algorithms/persistent_st.cpp

        tests/tests.cpp)

cmake_minimum_required(VERSION 3.26)
project(Lab1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all -Werror"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

add_executable(Lab1 main.cpp
        file_work.cpp
        prints.cpp
        data_base_functions.cpp
        backup.cpp)

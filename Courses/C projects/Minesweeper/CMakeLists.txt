cmake_minimum_required(VERSION 3.24)
project(Minesweeper0 C)

set(CMAKE_C_STANDARD 17)

add_executable(Minesweeper0 main.c generation.c game.c solution.c bot.c)

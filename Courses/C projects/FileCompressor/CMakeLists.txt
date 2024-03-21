cmake_minimum_required(VERSION 3.24)
project(Files C)

set(CMAKE_C_STANDARD 23)

add_executable(Files main.c UserInteraction.c Haffman.c Compressor.c Decompressor.c)

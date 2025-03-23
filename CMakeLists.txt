cmake_minimum_required(VERSION 3.28)
project(topdown_game)

set(CMAKE_CXX_STANDARD 17)



add_executable(topdown_game main.cpp
        lib/nuklear.h
        lib/raylib-nuklear.h
        lib/raylib-nuklear-font.h)

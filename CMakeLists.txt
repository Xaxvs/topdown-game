cmake_minimum_required(VERSION 3.28)
project(topdown_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(
        topdown_game
        main.cpp
        raylib-nuklear.cpp 
        editor.cpp
        Player.cpp
        units.cpp
        Enemies.cpp
)

target_include_directories(topdown_game PUBLIC lib/raylib/raylib/include box2d/include lib)
target_link_directories(topdown_game PUBLIC lib/raylib/raylib lib)
target_link_libraries(topdown_game raylib winmm box2d)

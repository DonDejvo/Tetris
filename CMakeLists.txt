cmake_minimum_required(VERSION 3.20)
project(Tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(sources
    src/figure.hpp
    src/figure.cpp
    src/window.hpp
    src/window.cpp
    src/argparser.hpp
    src/argparser.cpp
    src/animation.hpp
    src/animation.cpp
    src/game.hpp
    src/game.cpp
    src/utils.hpp
    src/utils.cpp
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_library(Catch2 STATIC vendor/catch/src/catch.cpp)
target_include_directories(Catch2 PUBLIC vendor/catch/include)

add_executable(Tetris
    ${sources}
    src/main.cpp)

target_link_libraries(Tetris PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

add_executable(TetrisTests
    ${sources}
    src/tests.cpp)

target_link_libraries(TetrisTests PRIVATE Catch2)
target_include_directories(TetrisTests PRIVATE vendor/catch/include)
target_compile_definitions(TetrisTests PRIVATE CATCH_CONFIG_MAIN)
target_link_libraries(TetrisTests PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

enable_testing()
add_test(NAME stage-1 COMMAND TetrisTests)

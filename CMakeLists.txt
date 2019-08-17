cmake_minimum_required(VERSION 3.6)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)
include_directories(C:/dev/libs/glm)

add_executable(GameOfLife main.cpp)
target_link_libraries(GameOfLife sfml-graphics-d sfml-system-d sfml-window-d)
target_link_libraries(GameOfLife opengl32)
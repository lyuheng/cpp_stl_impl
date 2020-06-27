cmake_minimum_required(VERSION 3.5)

include_directories (".")
project(RB_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RB_tree main.cpp)

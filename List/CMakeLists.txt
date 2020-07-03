cmake_minimum_required(VERSION 3.5)


include_directories (".")
project(List LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(List main.cpp)

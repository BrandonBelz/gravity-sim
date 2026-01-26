cmake_minimum_required(VERSION 3.20)

project(gravity_sim
    VERSION 0.1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
    core
    src/physics.cpp
)

target_include_directories(core
    PUBLIC src
)

enable_testing()
add_subdirectory(tests)

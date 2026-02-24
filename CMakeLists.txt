cmake_minimum_required(VERSION 3.20)
project(gravity_sim VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

add_library(core
    src/physics.cpp
    src/raylib_wrap.cpp
)
target_link_libraries(core raylib)
target_include_directories(core PUBLIC src)

add_executable(gravity src/main.cpp)
target_link_libraries(gravity core)

enable_testing()
add_subdirectory(tests)

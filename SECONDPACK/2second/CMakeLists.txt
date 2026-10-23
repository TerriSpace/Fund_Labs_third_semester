cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER /usr/bin/gcc-13)

project(MyProject C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(done main.c functions.c)

target_compile_definitions(done PRIVATE _POSIX_C_SOURCE=200809L)

target_compile_options(done PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(done PRIVATE m)
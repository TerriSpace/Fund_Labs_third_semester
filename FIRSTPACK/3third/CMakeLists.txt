cmake_minimum_required(VERSION 3.10)
project(MyProject C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(done main.c functions.c)

# Библиотеки
target_link_libraries(done m)

set(CMAKE_C_COMPILER /usr/bin/gcc-13)
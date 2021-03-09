cmake_minimum_required(VERSION 3.17)
project(hello)

set(CMAKE_CXX_STANDARD 14)

add_executable(hello main.cpp)

set(CMAKE_C_EXTENSIONS OFF)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # lots of warnings - you should fix them
    add_compile_options(-Wall -Wextra -pedantic)
endif()
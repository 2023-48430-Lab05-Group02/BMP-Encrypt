cmake_minimum_required(VERSION 3.23)
project(untitled C)

add_compile_options(-Wall -Werror)

set(CMAKE_C_STANDARD 99)

include_directories(.)
add_executable(untitled main.c bmp.c encryption.c input.c rle.c)

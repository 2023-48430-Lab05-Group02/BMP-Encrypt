cmake_minimum_required(VERSION 3.23)
project(untitled C)

add_compile_options(-Wall -Werror)

set(CMAKE_C_STANDARD 99)

include_directories(./src)
add_executable(untitled src/main.c src/bmp.c src/encryption.c src/input.c src/rle.c)

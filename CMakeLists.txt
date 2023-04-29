cmake_minimum_required(VERSION 3.23)
project(untitled C)

add_compile_options(-Wall -Werror -Wextra -Wformat-security -Wduplicated-cond
        -Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init
        -Wlogical-not-parentheses -Wnull-dereference -Wno-unused-variable
        -Wno-unused-parameter)

set(CMAKE_C_STANDARD 99)

include_directories(./src)
add_executable(untitled src/main.c src/bmp.c src/encryption.c src/input.c src/rle.c)

target_link_libraries(untitled PRIVATE m)

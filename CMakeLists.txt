# -- BMP Encrypt CMake Build File --
# This file describes build parameters for the BMP encrypt project.
# Running cmake . in the directory will produce a makefile that can
# be run using "make".

# Set project name as bmpe
cmake_minimum_required(VERSION 3.23)
project(bmpe C)

# Additional GCC Compile options - help prevent as many C issues as possible on compile time.
add_compile_options(-Wall -Werror -Wextra -Wformat-security -Wduplicated-cond
        -Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init
        -Wlogical-not-parentheses -Wnull-dereference -Wno-unused-variable
        -Wno-unused-parameter)

# C Version - C99 in use for modern feature such as inline variable definitions for loops.
set(CMAKE_C_STANDARD 99)

# Sanitizers - Verify any memory leaks.
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -g")

# Debug Mode - Override debug build without IDE.
#set(CMAKE_BUILD_TYPE Debug)

# Include paths.
include_directories(./src ./src/datatypes ./src/util)

# C Source files.
add_executable(bmpe src/main.c src/bmp.c src/encryption.c src/input.c
        src/rle.c src/datatypes/directory_tree.c src/util/realloc.c
        src/int.c src/cmd.c)

# Link libraries as required.
target_link_libraries(bmpe PRIVATE m)
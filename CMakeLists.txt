cmake_minimum_required(VERSION 3.23)
project(untitled C)

add_compile_options(-Wall -Werror -Wextra -Wformat-security -Wduplicated-cond
        -Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init
        -Wlogical-not-parentheses -Wnull-dereference -Wno-unused-variable
        -Wno-unused-parameter)

set(CMAKE_C_STANDARD 99)

# Sanitizers
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -g")

# Debug Mode
#set(CMAKE_BUILD_TYPE Debug)

include_directories(./src ./src/datatypes)
add_executable(untitled src/main.c src/bmp.c src/encryption.c src/input.c
               src/rle.c src/datatypes/directory_tree.c src/util/realloc.c
               src/int.c src/cmd.c)

target_link_libraries(untitled PRIVATE m)

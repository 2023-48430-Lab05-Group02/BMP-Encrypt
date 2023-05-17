//
// Created by Macauley Lim on 17/05/23.
//

#include "datatypes/short_sizes.h"

#ifndef DIRECTORY_TREE_H
#define DIRECTORY_TREE_H

// Structs
typedef struct file {
    char* name;
    struct directory* parent;
} file_t;

typedef struct directory {
    char* name;
    struct directory* parent;

    struct directory* subdirectories;
    u32_t s_length;
    u32_t s_capacity;

    struct file* files;
    u32_t f_length;
    u32_t f_capacity;
} directory_t;

// Return structs
typedef struct file_list {
    file_t* files;
    u32_t files_length;
} file_list_t;

typedef struct directory_list {
    directory_t* directories;
    u32_t directories_length;
} directory_list_t;

// Function Declarations
directory_t directory_tree_new(char* raw_file_path);
directory_t directory_tree_new_from_dir_path(char* dir_path);
directory_t* directory_tree_new_directory(char* name, directory_t* parent);
file_t* directory_tree_new_file(char* name, directory_t* parent);
void directory_tree_get_file_path(file_t* file, char* name, u32_t max_size);
file_list_t directory_tree_get_files(directory_t* parent);
directory_list_t directory_tree_get_subdirectories(directory_t* parent);
#endif //DIRECTORY_TREE_H

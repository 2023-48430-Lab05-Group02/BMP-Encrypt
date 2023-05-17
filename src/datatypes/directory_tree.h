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
    struct directory_tree* tree;

    struct directory* subdirectories;
    u32_t subdirectories_length;

    struct file* files;
    u32_t files_length;
} directory_t;

typedef struct directory_tree {
    char* file_path;
    struct directory root_directory;
} directory_tree_t;

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
directory_tree_t directory_tree_new(char* raw_file_path);
directory_tree_t directory_tree_new_from_dir_path(char* dir_path);
void directory_tree_new_directory(directory_t* parent);
void directory_tree_new_file(directory_t* parent);
void directory_tree_get_file_path(file_t* file);
file_list_t directory_tree_get_files(directory_t* parent);
directory_list_t directory_tree_get_subdirectories(directory_t* parent);

#endif //DIRECTORY_TREE_H

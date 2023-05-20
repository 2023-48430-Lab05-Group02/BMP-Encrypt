// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// ---------------------------directory_tree.h----------------------------------

// Include Guard
#ifndef DIRECTORY_TREE_H
#define DIRECTORY_TREE_H

// Standard Library Includes
#ifdef __linux__
#include <dirent.h> // PATH_MAX
#endif

// Other Includes
#include "datatypes/short_sizes.h"

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------
/*
 * A struct representing a file in the directory tree.
 * It has a name, and parent directory.
 */
typedef struct file {
    char name[PATH_MAX];
    struct directory* parent;
} file_t;

/*
 * A struct representing a directory in the directory tree.
 * This may also be root directory if parent = NULL.
 * Each directory has:
 * - A name (equivalent to its d_name).
 * - A parent, which may be null for the root directory.
 * - An array of subdirectories with accompanying length and capacity.
 * - An array of files with accompanying length and capacity.
 */
typedef struct directory {
    char name[PATH_MAX];
    struct directory* parent;

    struct directory* subdirectories;
    u32_t s_length;
    u32_t s_capacity;

    struct file* files;
    u32_t f_length;
    u32_t f_capacity;
} directory_t;

//------------------------------------------------------------------------------
// Function Return Type Structs
//------------------------------------------------------------------------------
/*
 * A list of files and accompanying length for iteration.
 */
typedef struct file_iter {
    file_t** files;
    u32_t files_length;
    u32_t files_capacity;
} file_iter_t;

/*
 * A list of directories and accompanying length for iteration.
 */
typedef struct directory_iter {
    directory_t* directories;
    u32_t directories_length;
} directory_iter_t;

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
/*
 * Create a new (empty) directory tree.
 * Returns a directory_t root directory, with default capacity but no files
 * or subdirectories.
 */
void directory_tree_new(directory_t* dir, char* raw_file_path);
/*
 * Creates a new directory tree populated from a directory path dir_path.
 */
void directory_tree_new_from_dir_path(directory_t* dir, char* dir_path);
/*
 * Creates a new directory in the directory path with name and parent specified.
 */
directory_t* directory_tree_new_directory(char* name, directory_t* parent);
/*
 * Creates a new file with name specified in directory parent.
 */
file_t* directory_tree_new_file(char* name, directory_t* parent);
/*
 * Get the full file path of a file and populate it into name as long as it
 * will fit within the max_size specified.
 */
void directory_tree_get_file_path(file_t* file, char* name);
/*
 * Get the full file path of a directory and populate it into name as long as it
 * will fit within the max_size specified.
 */
void directory_tree_get_directory_path(directory_t* director, char* name);
/*
 * Get a list of files with parent directory parent.
 */
void directory_tree_get_files(file_iter_t* iter, directory_t* parent);
/*
 * Recursive iterator that includes all files from below the directory using
 * their full path.
 */
void directory_tree_get_files_recursive(file_iter_t* iter, directory_t* parent);
/*
 * Get a list of subdirectories for parent directory parent.
 */
void directory_tree_get_subdirectories(directory_iter_t* iter, directory_t* parent);
/*
 * Deconstruct the tree freeing all memory to prevent memory leaks.
 */
void directory_tree_deconstructor(directory_t* directory);

#endif //DIRECTORY_TREE_H

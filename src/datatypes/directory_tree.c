//
// Created by Macauley Lim on 17/05/23.
//

// Standard Library Includes
#include <stdlib.h> // exit
#include <stdio.h> // printf
#include <string.h> // strcmp
#ifdef __linux__
#include <dirent.h> // DIR, opendir
#include <sys/stat.h> // S_ISDIR
#elif _WIN64
#include <windows.h>
#endif

// Public API Includes
#include "directory_tree.h"

// Internal use structs
typedef struct file_directory_list {
    file_list_t files;
    directory_list_t directories;
} file_directory_list_t;

// Function Definitions
directory_tree_t directory_tree_new(char* raw_file_path) {
    directory_t directory = {

    };
    directory_tree_t tree = {
        ,
    };
    return tree;
}
directory_tree_t directory_tree_new_from_dir_path(char* dir_path) {

}
void directory_tree_new_directory(directory_t* parent) {

}
void directory_tree_new_file(directory_t* parent) {

}
void directory_tree_get_file_path(file_t* file) {

}
file_list_t directory_tree_get_files(directory_t* parent) {

}
directory_list_t directory_tree_get_subdirectories(directory_t* parent) {

}
// Internal use functions
#ifdef _WIN64
void dir_to_dir_tree_recursive(char* path, directory_tree_t* tree) {
}
#elif __linux__
void dir_to_dir_tree_recursive(char* path, directory_tree_t* tree) {
    DIR* directory;
    struct dirent* object;
    struct stat object_info;

    directory = opendir(path);
    // This should never occur as the directory should be checked by input code.
    if (directory == NULL) {
        printf("Unrecoverable error in directory tree.");
        exit(EXIT_FAILURE);
    }

    // Iterate the directory.
    while ((object = readdir(directory)) != NULL) {
        // Skip .. and . directories
        if (strcmp(object->d_name, ".") == 0
            || strcmp(object->d_name, "..") == 0)
        {
            continue;
        }

        // Get information about the object.
        if (stat(fullpath, &object_info) < 0) {
            printf("Could not get file status: %s\n", fullpath);
            continue;
        }

        // Handle recursion.
        if (S_ISDIR(object_info.st_mode)) {
            // Directory
            dir_to_dir_tree_recursive(fullpath, )
        }
    }
}
#endif

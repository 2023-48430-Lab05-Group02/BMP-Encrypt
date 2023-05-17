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

// Other Includes
#include "../util/realloc.h"

// Internal use structs
typedef struct file_directory_list {
    file_list_t files;
    directory_list_t directories;
} file_directory_list_t;

// Internal use function declarations
void dir_to_dir_tree_recursive(directory_t* tree_dir);

// Function Definitions
directory_t directory_tree_new(char* raw_file_path) {
    directory_t directory = {
        raw_file_path,
        NULL,
        malloc(sizeof(directory_t*) * 3),
        0,
        3,
        malloc(sizeof(file_t*) * 16),
        0,
        16
    };
    return directory;
}
directory_t directory_tree_new_from_dir_path(char* dir_path) {
    directory_t empty_dir = directory_tree_new(dir_path);
    dir_to_dir_tree_recursive(&empty_dir);
    return empty_dir;
}
directory_t* directory_tree_new_directory(char* name, directory_t* parent) {
    if (parent->s_capacity == parent->s_length) {
        safe_realloc(parent->subdirectories, parent->s_capacity * 2);
    }
    parent->subdirectories[parent->s_length] =
            directory_tree_new(name);
    parent->s_length++;
    return &parent->subdirectories[parent->s_length + 1];
}
file_t* directory_tree_new_file(char* name, directory_t* parent) {
    file_t* file = malloc(sizeof(file_t));
    return file;
}
void directory_tree_get_file_path(file_t* file, char* name, u32_t max_size) {

}
file_list_t* directory_tree_get_files(directory_t* parent) {
    file_list_t* file = malloc(sizeof(file_list_t));
    return file;

}
directory_list_t* directory_tree_get_subdirectories(directory_t* parent) {
    directory_list_t* file = malloc(sizeof(directory_list_t));
    return file;
}
// Internal use functions
#ifdef _WIN64
void dir_to_dir_tree_recursive(directory_t* tree_dir) {
}
#elif __linux__
void dir_to_dir_tree_recursive(directory_t* tree_dir) {
    DIR* directory;
    struct dirent* object;
    struct stat object_info;

    directory = opendir(tree_dir->name);
    // This should never occur as the directory should be checked by input code.
    if (directory == NULL) {
        printf("Unrecoverable error in directory tree.\n");
        exit(EXIT_FAILURE);
    }

    // Iterate the directory.
    while ((object = readdir(directory)) != NULL)
    {
        // Get the full path of the file.
        char fullpath[PATH_MAX];
        snprintf(fullpath, PATH_MAX, "%s/%s", tree_dir->name, object->d_name);

        // Skip .. and . directories
        if (strcmp(object->d_name, ".") == 0
            || strcmp(object->d_name, "..") == 0)
        {
            continue;
        }

        // Get information about the object.
        if (stat(fullpath, &object_info) < 0)
        {
            printf("File at %s could not be read. Skipping.\n", fullpath);
            continue;
        }

        // Handle recursion.
        if (S_ISDIR(object_info.st_mode))
        {
            // Directory Ahoy.
            dir_to_dir_tree_recursive(directory_tree_new_directory(object->d_name, tree_dir));
        }
        else
        {
            // It's a file.
            directory_tree_new_file(object->d_name, tree_dir);
        }
    }
    closedir(directory);
}
#endif

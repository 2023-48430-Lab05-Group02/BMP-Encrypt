// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// ---------------------------directory_tree.c----------------------------------

// Standard Library Includes
#include <stdlib.h> // exit malloc
#include <stdio.h> // printf snprintf
#include <string.h> // strcmp strcpy strcat
/*
 * This library include requires some more explanation. The primary purpose of
 * it is to allow the code to access the subdirectories of a directory to bulk
 * process all the files in the structure for the purposes of encryption or
 * decryption.
 * For Linux, the following libraries are in use beyond the stdlib:
 * dirent.h - For directory handling using the DIR type.
 * sys/stat.h - To get information about the content of objects inside a DIR.
 * For Windows, the situation is significantly different due to the monolithic
 * status of the windows "windows.h" library. The specific functions used are
 * noted in the comment after the function include.
 */
// Linux only includes
#ifdef __linux__
#include <dirent.h> // DIR, opendir, closedir, PATH_MAX
#include <sys/stat.h> // S_ISDIR, stat

// Windows only includes
#elif _WIN64 // __linux__
#include <windows.h> // FindFirstFileA, FindNextFile, WIN32_FIND_DATA, FindClose

#endif // __linux__,_WIN64

// Public API Includes
#include "directory_tree.h"

// Other Includes
#include "../util/realloc.h" // safe_realloc
#include "bool.h" // true false bool

//------------------------------------------------------------------------------
// Private Function Declarations
//------------------------------------------------------------------------------
/*
 * Takes an empty directory_t root directory (parent = NULL) and populate it
 * with the files and directories physically at that location in the system.
 */
void dir_to_dir_tree_recursive(directory_t* tree_dir);

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void directory_tree_new(directory_t* dir, char* raw_file_path) {
    strcpy(dir->name, raw_file_path);
    dir->parent = NULL;

    dir->subdirectories = malloc(sizeof(directory_t) * 3);
    dir->s_length = 0;
    dir->s_capacity = 3;

    dir->files = malloc(sizeof(file_t) * 16);
    dir->f_length = 0;
    dir->f_capacity = 16;
}
void directory_tree_new_from_dir_path(directory_t* dir, char* dir_path) {
    directory_tree_new(dir,dir_path);
    dir_to_dir_tree_recursive(dir);
}
directory_t* directory_tree_new_directory(char* name, directory_t* parent) {
    if (parent->s_capacity == parent->s_length)
    {
        parent->subdirectories = safe_realloc(parent->subdirectories,
                     parent->s_capacity * 2 * sizeof(directory_t));
        parent->s_capacity *= 2;
    }

    directory_t* dir = &parent->subdirectories[parent->s_length];

    strcpy(dir->name, name);
    dir->parent = parent;

    dir->subdirectories = malloc(sizeof(directory_t) * 3);
    dir->s_length = 0;
    dir->s_capacity = 3;

    dir->files = malloc(sizeof(file_t) * 16);
    dir->f_length = 0;
    dir->f_capacity = 16;

    parent->s_length++;
    return &parent->subdirectories[parent->s_length - 1];
}
file_t* directory_tree_new_file(char* name, directory_t* parent) {
    if (parent->f_capacity == parent->f_length)
    {
        parent->files = safe_realloc(parent->files,
                     parent->f_capacity * 2 * sizeof(file_t));
        parent->f_capacity *= 2;
    }
    file_t file;
    strcpy(file.name, name);
    file.parent = parent;

    parent->files[parent->f_length] = file;

    parent->f_length++;
    return &parent->files[parent->f_length-1];
}
void directory_tree_get_file_path(file_t* file, char* name) {
    bool reached_root = false;
    directory_t* parent = file->parent;
    char left[PATH_MAX], right[PATH_MAX];

    strcpy(right, file->name);

        while (!reached_root)
        {
            strcpy(left, parent->name);
            #ifdef _WIN64
            strcat(left, "\\");
            #elif __linux__ // _WIN64
            strcat(left, "/");
            #endif // _WIN64,__linux__
            strcat(left, right);
            strcpy(right, left);
            if (parent->parent == NULL)
            {
                reached_root = true;
            }
            else
            {
                parent = parent->parent;
            }
        }

    strcpy(name, right);
}
void directory_tree_get_directory_path(directory_t* directory, char* name) {
    bool reached_root = false;
    directory_t* parent = directory->parent;
    char left[PATH_MAX], right[PATH_MAX];

    strcpy(right, directory->name);

    while (!reached_root) {
        if (parent != NULL)
        {
            strcpy(left, parent->name);
            #ifdef _WIN64
            strcat(left, "\\");
            #elif __linux__ // _WIN64
            strcat(left, "/");
            #endif // _WIN64,__linux__
            strcat(left, right);
            strcpy(right, left);
            parent = parent->parent;
        }
        else
        {
            reached_root = true;
        }
    }
    strcpy(name, right);
}
void directory_tree_get_files(file_iter_t* iter, directory_t* parent) {
    iter->files = &parent->files;
    iter->files_length = parent->f_length;
}
void directory_tree_get_files_recursive(file_iter_t* iter, directory_t* dir) {
    // Realloc if necessary
    if (iter->files_capacity <= (iter->files_length + dir->f_length))
    {
        iter->files = safe_realloc(iter->files, iter->files_capacity
                                 + dir->f_length * 2 * (u32_t) sizeof(file_t*));
        iter->files_capacity = iter->files_capacity * 2;
    }
    // Add files from this dir.
    for (u32_t i = 0; i < dir->f_length; i++)
    {
        iter->files[i + iter->files_length] = &dir->files[i];
    }
    iter->files_length += dir->f_length;
    // Iterate sub directories
    for (u32_t i = 0; i < dir->s_length; i++)
    {
        directory_tree_get_files_recursive(iter, &dir->subdirectories[i]);
    }
}
void directory_tree_get_subdirectories(directory_iter_t* iter, directory_t* parent) {
    iter->directories = parent->subdirectories;
    iter->directories_length = parent->s_length;
}
void directory_tree_deconstructor(directory_t* directory) {
    free(directory->files);
    for(u32_t i = 0; i < directory->s_length; i++)
    {
        directory_tree_deconstructor(&directory->subdirectories[i]);
    }
    free(directory->subdirectories);
    // Free only if not root directory since that is always stack allocated.
    if (directory->parent != NULL) {
        free(directory);
    }
}
//------------------------------------------------------------------------------
// Private Function Definitions
//------------------------------------------------------------------------------
#ifdef _WIN64
void dir_to_dir_tree_recursive(directory_t* tree_dir) {
    char find_path[PATH_MAX];
    char dir_path[PATH_MAX];
    WIN32_FIND_DATA find_info;
    HANDLE find;

    // Add \\ to the path.
    directory_tree_get_directory_path(tree_dir, dir_path);
    snprintf(find_path, PATH_MAX, "%s\\*", dir_path);

    // Begin the find file.
    find = FindFirstFileA(find_path, &find_info);
    // This should never occur as the directory should be checked by input code.
    if (find == INVALID_HANDLE_VALUE)
    {
        u32_t error = GetLastError();
        printf("Unrecoverable error in directory tree: error %u.\n", error);
        exit(EXIT_FAILURE);
    }

    // Handle the rest of the files...
    do
    {
        // Skip .. and . directories
        if (strcmp(find_info.cFileName, ".") == 0
            || strcmp(find_info.cFileName, "..") == 0)
        {
            continue;
        }

        // Handle File or Directory.
        if (find_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            dir_to_dir_tree_recursive(directory_tree_new_directory(
                    find_info.cFileName, tree_dir));
        }
        else
        {
            directory_tree_new_file(find_info.cFileName, tree_dir);
        }
    }
    while (FindNextFile(find, &find_info));

    FindClose(find);
}
#elif __linux__ // _WIN64
void dir_to_dir_tree_recursive(directory_t* tree_dir) {
    DIR* directory;
    char dir_path[PATH_MAX-256];
    struct dirent* object;
    struct stat object_info;

    // Get the full directory path
    directory_tree_get_directory_path(tree_dir, dir_path);

    directory = opendir(dir_path);
    // This should never occur as the directory should be checked by input code.
    if (directory == NULL) {
        printf("Unrecoverable error in directory tree.\n");
        exit(EXIT_FAILURE);
    }

    // Iterate the directory.
    while ((object = readdir(directory)) != NULL)
    {
        // Get the full path of the file.
        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", dir_path, object->d_name);

        // Skip .. and . directories
        if (strcmp(object->d_name, ".") == 0
            || strcmp(object->d_name, "..") == 0)
        {
            continue;
        }

        // Get information about the object.
        if (stat(full_path, &object_info) < 0)
        {
            printf("File at %s could not be read. Skipping.\n", full_path);
            continue;
        }

        // Handle File or Directory.
        if (S_ISDIR(object_info.st_mode))
        {
            // Directory Recurse Ahoy.
            dir_to_dir_tree_recursive(directory_tree_new_directory(
                    object->d_name, tree_dir));
        }
        else
        {
            // It's a file.
            directory_tree_new_file(object->d_name, tree_dir);
        }
    }
    closedir(directory);
}
#endif // _WIN64,__linux__

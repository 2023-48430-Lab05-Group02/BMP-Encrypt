// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// -----------------------------------cmd.c-------------------------------------

// Standard Library Includes
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <dirent.h> // PATH_MAX
#endif

// Public API Includes
#include "cmd.h"

// Other Includes
#include "datatypes/directory_tree.h"
#include "datatypes/short_sizes.h"
#include "datatypes/bool.h"

#include "main.h"

// Static Defines
#define RUNTIME_DEBUG

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void main_batch(ProgramState_t state) {
    // Tree
    directory_t root_dir;
    root_dir = directory_tree_new_from_dir_path(state.input_file_name);
    file_iter_t complete_iter = directory_tree_get_files_recursive(&root_dir);

    // Variables
    char input_file_name[PATH_MAX];
    char output_file_name[PATH_MAX];
    FILE* input_file;
    FILE* output_file;

    printf("Beginning batch processing at path: %s", root_dir.name);

    if(state.encrypt_mode) // Handle Encryption
    {
        for (u32_t i = 0; i < complete_iter.files_length; i++)
        {
            directory_tree_get_file_path(complete_iter.files[i], input_file_name);
            strcpy(output_file_name, input_file_name);
            strcat(output_file_name, "e");

            input_file = fopen(state.input_file_name, "r");
            output_file = fopen(output_file_name, "w");

            encrypt_file(input_file, output_file,
                         state.encryption_key,
                         !state.force_nonfatal_mode,
                         state.compress_mode);
        }
    }
    else if (state.decrypt_mode) // Handle Decryption
    {
        for (u32_t i = 0; i < complete_iter.files_length; i++)
        {
            strcpy(output_file_name, complete_iter.files[i]->name);
            strcat(output_file_name, "e");

            input_file = fopen(state.input_file_name, "r");
            output_file = fopen(output_file_name, "w");

            decrypt_file(input_file, output_file,
                         state.encryption_key,
                         !state.force_nonfatal_mode,
                         state.compress_mode);
        }
    }
    else if (state.compress_mode) // Handle compression
    {
        for (u32_t i = 0; i < complete_iter.files_length; i++)
        {
            strcpy(output_file_name, complete_iter.files[i]->name);
            strcat(output_file_name, "e");

            input_file = fopen(state.input_file_name, "r");
            output_file = fopen(output_file_name, "w");

            compress_file(input_file, output_file, !state.force_nonfatal_mode);
        }
    }
    else if (state.decompress_mode) // Handle decompression
    {
        for (u32_t i = 0; i < complete_iter.files_length; i++)
        {
            strcpy(output_file_name, complete_iter.files[i]->name);
            strcat(output_file_name, "e");

            input_file = fopen(state.input_file_name, "r");
            output_file = fopen(output_file_name, "w");

            decompress_file(input_file, output_file, !state.force_nonfatal_mode);
        }
    }
    directory_tree_deconstructor(&root_dir);
}
void main_single(ProgramState_t state) {
    // Variables
    char output_file_name[PATH_MAX];
    FILE* input_file = NULL;
    FILE* output_file = NULL;

    if (state.force_nonfatal_mode) // Note if ignore nonfatal is being used.
    {
        printf("Ignoring non-fatal BMP file format errors.\n");
    }

    if (state.encrypt_mode) // Handle encryption
    {
        #ifdef RUNTIME_DEBUG
        printf("Attempting file read from %s.\n", state.input_file_name);
        #endif

        strcpy(output_file_name, state.input_file_name);
        strcat(output_file_name, "e");

        input_file = fopen(state.input_file_name, "r");
        output_file = fopen(output_file_name, "w");

        encrypt_file(input_file, output_file, state.encryption_key,
                     !state.force_nonfatal_mode, state.compress_mode);
    }
    else if (state.decrypt_mode) // Handle Decryption
    {

    }
    else if (state.compress_mode) // Handle compression
    {

    }
    else if (state.decompress_mode) // Handle decompression
    {

    }
}
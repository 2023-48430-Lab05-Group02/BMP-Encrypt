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
#include "datatypes/short_sizes.h"
#include "datatypes/bool.h"

#include "main.h"

// Static Defines
#define RUNTIME_DEBUG

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void main_batch(ProgramState_t state) {

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
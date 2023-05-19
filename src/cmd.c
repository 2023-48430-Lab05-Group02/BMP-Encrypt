// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// -----------------------------------cmd.c-------------------------------------

// Standard Library Includes
#include <stdio.h>

// Public API Includes
#include "cmd.h"

// Other Includes
#include "datatypes/short_sizes.h"
#include "datatypes/bool.h"

//------------------------------------------------------------------------------
// Private Function Declarations
//------------------------------------------------------------------------------
void encrypt_file(FILE* input, FILE* output, u32_t* key, bool strict_verify, bool compress);
void decrypt_file(FILE* input, FILE* output, u32_t* key, bool strict_verify, bool compress);
void compress_file(FILE* input, FILE* output, bool strict_verify);
void decompress_file(FILE* input, FILE* output, bool strict_verify);

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void main_batch() {

}
void main_single() {
    // Note if ignore nonfatal is being used.
    if (ignore_nonfatal)
    {
        printf("Ignoring non-fatal BMP file format errors.\n");
    }

    // Handle encryption
    if (encrypt_mode)
    {
#ifdef RUNTIME_DEBUG
        printf("Attempting file read from %s.\n", input_file_name);
#endif

        strcpy(output_file_name, input_file_name);
        strcat(output_file_name, "e");

        input_file = fopen(input_file_name, "r");
        output_file = fopen(output_file_name, "w");

        encrypt_file(input_file, output_file, encryption_key, !ignore_nonfatal, compress_mode);
    }
        // Handle Decryption
    else if (decrypt_mode)
    {

    }
        // Handle compression
    else if (compress_mode)
    {

    }
        // Handle decompression
    else if (decompress_mode)
    {

    }
}
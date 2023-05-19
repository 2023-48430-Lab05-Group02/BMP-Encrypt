// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// ----------------------------Compile Instructions-----------------------------
//
// -----------------------------------main.c------------------------------------

// Include guards
#ifndef MAIN_H
#define MAIN_H

// Standard Library Includes
#ifdef __linux__
#include <dirent.h> // PATH_MAX
#elif _WIN64
#include <stdlib.h> // PATH_MAX
#endif

// Other includes
#include "datatypes/bool.h"
#include "short_sizes.h"

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------
typedef struct ProgramState {
    // Program Operation State
    bool interactive_mode;
    bool encrypt_mode;
    bool decrypt_mode;
    bool compress_mode;
    bool decompress_mode;
    bool batch_mode;
    bool force_nonfatal_mode;

    // Input file
    bool input_file_present;
    char input_file_name[PATH_MAX];

    // Encryption Key
    u32_t encryption_key;
    bool encryption_key_present;
} ProgramState_t;


#endif //MAIN_H

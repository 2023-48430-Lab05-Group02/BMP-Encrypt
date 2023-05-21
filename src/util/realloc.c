// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Benjamin Hudson
// --------------------------------realloc.c------------------------------------

// Standard Library Includes
#include <stdlib.h> // realloc malloc
#include <string.h> // memcpy

// Public API Includes
#include "realloc.h"

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void* safe_realloc(void* pointer, u32_t size) {
    void* output;
    output = realloc(pointer, size);

    if (output == NULL){
        output = malloc(size);
        memcpy(output, pointer, sizeof(*pointer));
    }
    return output;
}

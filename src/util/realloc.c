// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Benjamin Hudson
// --------------------------------realloc.c------------------------------------

// Standard Library Includes
#include <stdlib.h>
#include <string.h>

// Public API Includes
#include "realloc.h"

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void safe_realloc(void* pointer, u32_t size) {
    pointer = realloc(pointer, size);

    if (pointer == NULL){
        void* temp_ptr;
        temp_ptr = malloc(size);
        memcpy(temp_ptr, pointer, sizeof(*pointer));
        pointer = temp_ptr;
    }
}

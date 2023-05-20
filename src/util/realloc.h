// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// --------------------------------realloc.h------------------------------------

// Include Guards
#ifndef REALLOC_H
#define REALLOC_H

// Other Includes
#include "datatypes/short_sizes.h"

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
/*
 * A function to wrap realloc to make it safe. E.g. if a realloc is not
 * possible it will malloc a new space and copy the data to the new space.
 */
void safe_realloc(void* pointer, u32_t size);

#endif //REALLOC_H

// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// ---------------------------------result.h------------------------------------

// Include Guard
#ifndef RESULT_H
#define RESULT_H

// Other Includes
#include "bool.h" // bool

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------
/*
 * A way of representing whether a function has returned a valid result or not.
 * If ok is false, data must contain a char array with an error message.
 * Else, data is the return value of the function.
 */
typedef struct result_struct {
    bool ok;
    void* data;
} result_t;

#endif //RESULT_H

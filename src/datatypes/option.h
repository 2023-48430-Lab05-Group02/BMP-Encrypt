// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// ----------------------------------option.h-----------------------------------

// Include guard
#ifndef OPTION_H
#define OPTION_H

// Other Includes
#include "bool.h" // bool

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------
/*
 * A way of representing an optional. Either the value is present or not.
 * This helps to ensure the safety of the code by ensuring allowing code to
 * ensure a value is present or handle if it isn't.
 * Data is only present if present is true. Otherwise, data is a nullptr.
 */
typedef struct option_struct {
    bool present;
    void* data;
} option_t;

#endif //OPTION_H

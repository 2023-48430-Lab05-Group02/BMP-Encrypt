// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// -----------------------------------bool.h------------------------------------

// Include guard
#ifndef BOOL_H
#define BOOL_H

//------------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------------
/*
 * Is either true (1) or false (0). Just a nicer way to interact with these kind
 * of on or off values without being able to use the stdbool.h library.
 */
typedef enum bool_enum {
    false = 0,
    true = 1
} bool;

#endif //BOOL_H

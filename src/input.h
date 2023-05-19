// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Joseph Rico
// ----------------------------------input.h------------------------------------

// Include Guard
#ifndef INPUT_H
#define INPUT_H

// Standard Library Includes
#include <stdio.h>

// Other Includes
#include "datatypes/result.h"
#include "short_sizes.h"

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
/*
 * Prints the help menu to the stdio.
 */
void print_menu_help();
/*
 * Prints the interactive menu to the stdio.
 */
void print_menu_interactive();

/*
 * A special function for printing unsigned ints as a set of 0 and 1's e.g. binary output.
 */
void print_unsigned_int_binary(unsigned int num);

/*
 * These functions attempt to get user input of the specified type.
 * They contain logic to handle user input errors.
 * Returns:
 * result_t with ok = true if it succeeded and the specified data type.
 * Or if it failed, ok = false and a char[] message specifying why.
 */
// Returns result_t data of int. Inputs: min, max and out of range error msg.
i32_t input_number(int min, int max, char error_message[]);
// Returns result_t data of nullptr. Pass in the char* buffer.
void input_string(char* str, int max_length);
// Returns a bool.
bool input_bool();
// Returns result_t data of FILE*.
void input_file_read(FILE* file);
// Returns result_t data of FILE*.
void input_file_write(FILE* file);

#endif //INPUT_H
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File: Joseph Rico

#include <stdio.h>

#include "datatypes/result.h"

#ifndef INPUT_H
#define INPUT_H

/*
 * Prints the help menu to the stdio.
 */
void print_menu_help();
/*
 * Prints the interactive menu to the stdio.
 */
void print_menu_interactive();

/*
 * These functions attempt to get user input of the specified type.
 * They contain logic to handle user input errors.
 * Returns:
 * result_t with ok = true if it succeeded and the specified data type.
 * Or if it failed, ok = false and a char[] message specifying why.
 */
// Returns result_t data of int. Inputs: min, max and out of range error msg.
result_t input_number(int min, int max, char error_message[]);
// Returns result_t data of nullptr. Pass in the char* buffer.
result_t input_string(char* str[], int max_length);
// Returns result_t data of FILE*.
result_t file_read(char name[], int length);
// Returns result_t data of FILE*.
result_t file_write(char name[], int length);

#endif //INPUT_H
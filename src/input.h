// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include <stdio.h>

#include "datatypes/result.h"

#ifndef INPUT_H
#define INPUT_H

void print_menu_help();
void print_menu_interactive();

result input_number(int* value, int min, int max, char error_message[]);
result input_string(char* str[], int max_length);
result file_read(char name[], int length, FILE** file);
result file_write(char name[], int length, FILE** file);

#endif //INPUT_H
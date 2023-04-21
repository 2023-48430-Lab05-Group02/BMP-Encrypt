//
// Created by Macauley Lim on 20/04/23.
//
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
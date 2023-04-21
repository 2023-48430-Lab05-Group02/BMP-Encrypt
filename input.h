//
// Created by Macauley Lim on 20/04/23.
//
#include <stdio.h>

#ifndef INPUT_H
#define INPUT_H

void print_menu();

int input_number(int min, int max, char error_message[]);
int input_string(char* str[], int max_length);
int file_read(char name[], int length, FILE** file);
int file_write(char name[], int length, FILE** file);

#endif //INPUT_H
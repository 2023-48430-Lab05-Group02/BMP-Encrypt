// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "input.h"

void print_menu_help() {
    printf("Help Test");
}
void print_menu_interactive() {
    printf("Interactive Test");
}

void print_unsigned_int_binary(unsigned int num) {
    for (int i = sizeof(unsigned int) * 8 - 1; i >= 0; i--)
        printf("%d", (num >> i) & 1);
}

result_t input_number(int min, int max, char error_message[]) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}
result_t input_string(char* str[], int max_length) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}
result_t file_read(char name[]) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}
result_t file_write(char name[]) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

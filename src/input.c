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
result_t file_read(char name[], int length) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}
result_t file_write(char name[], int length) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

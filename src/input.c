// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Joseph Rico
// ----------------------------------input.c------------------------------------

// Standard Library Includes
#include <string.h>
#ifdef __linux__
#include <dirent.h> // PATH_MAX
#elif _WIN64
#include <stdlib.h> // PATH_MAX
#endif

// Public API
#include "input.h"

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void print_menu_help() {
    printf("------------BMP Encrypt------------\n"
           "");
}
void print_menu_interactive() {
    printf("Which action would you like to perform:\n");
    printf("1. Encrypt\n"
           "2. Decrypt\n"
           "3. Compress\n"
           "4. Decompress\n"
           "5. Info\n"
           "6. Force non-fatal BMP files to be processed\n"
           "7. Quit\n");
    printf("Select your choice >");
}

void print_unsigned_int_binary(unsigned int num) {
    for (int i = sizeof(unsigned int) * 8 - 1; i >= 0; i--)
        printf("%d", (num >> i) & 1);
}
// Note that the number in question may never be zero with this implementation.
i32_t input_number(int min, int max, char error_message[]) {
    bool valid = false;
    i32_t out;
    char num[11];

    while (!valid)
    {
        scanf("%11s", num);
        out = strtol(num, NULL, 10);
        if (out >= min && out <= max && out != 0)
        {
            valid = true;
        }
        else
        {
            printf("%s\n", error_message);
            printf("Please enter the value again >");
        }
    }
    printf("\n");
    return out;
}
void input_string(char* str, int max_length) {
    char format_string[10];
    snprintf(format_string, sizeof(format_string), "%%%ds", max_length);
    scanf(format_string, str);
    printf("\n");
}
bool input_bool() {
    bool out;
    bool valid = false;
    char state[6];

    while (!valid)
    {
        scanf("%5s", state);

        if (state[0] == '0' || strcmp(state, "false") == 0
                            || strcmp(state, "yes") == 0)
        {
            valid = true;
            out = false;
        }
        else if (state[0] == '1' || strcmp(state, "true") == 0
                                 || strcmp(state, "no") == 0)
        {
            valid = true;
            out = true;
        }
        else
        {
            printf("Invalid Boolean entered. Please enter the value"
                   " [0, false, no], [1, true, yes] again >");
        }
    }
    printf("\n");
    return out;
}
void input_file_read(FILE* file) {
    bool valid = false;
    char format_string[10];
    snprintf(format_string, sizeof(format_string), "%%%ds", PATH_MAX);
    char path[PATH_MAX];

    while (!valid)
    {
        scanf(format_string, path);
        file = fopen(path, "r");
        if (file != NULL)
        {
            valid = true;
        }
        else
        {
            printf("File could not be found. Please enter your file name"
                   " again >");
        }
    }
    printf("\n");
}
void input_file_write(FILE* file) {
    bool valid = false;
    char format_string[10];
    snprintf(format_string, sizeof(format_string), "%%%ds", PATH_MAX);
    char path[PATH_MAX];

    while (!valid)
    {
        scanf(format_string, path);
        file = fopen(path, "w");
        if (file != NULL)
        {
            valid = true;
        }
        else
        {
            printf("File could not be found. Please enter your file name"
                   "again >");
        }
    }
    printf("\n");
}

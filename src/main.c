// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// <Compilation Instructions Here, Other Notes>

// The 4 libraries we are allowed.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "datatypes/bool.h"
#include "datatypes/result.h"
#include "datatypes/option.h"

#include "bmp.h"
#include "encryption.h"
#include "input.h"
#include "rle.h"

int main(int argc, char* argv[]){
    // Initialize program
    bool interactive_mode = false;
    bool batch_mode = false;
    bool encrypt_mode = false;
    bool decrypt_mode = false;

    unsigned int* encryption_key = malloc(sizeof(unsigned int));

    // Process arguments
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "--interactive") == 0 ||
           strcmp(argv[i], "-I") == 0) {
            interactive_mode = true;
        }
        if(strcmp(argv[i], "--batch") == 0 || strcmp(argv[i], "-B") == 0) {
            batch_mode = true;
        }
        if(strcmp(argv[i], "--encrypt") == 0 || strcmp(argv[i], "-E") == 0) {
            encrypt_mode = true;
            i++;
        }
        if(strcmp(argv[i], "--decrypt") == 0 || strcmp(argv[i], "-D") == 0) {
            decrypt_mode = true;
        }
        if(strcmp(argv[i], "--key") == 0 || strcmp(argv[i], "-K") == 0) {
            memcpy(encryption_key, argv[i + 1], 4);
        }
        if(strcmp(argv[i], "--password") == 0 || strcmp(argv[i], "-P") == 0) {
            *encryption_key = fnv1a_hash(argv[i + 1]);
        }
    }

    if(!interactive_mode || !batch_mode || !encrypt_mode || !decrypt_mode) {
        print_menu_help();
    }

    // If in interactive mode, Enter the event loop
    while (interactive_mode) {
        // Print menu
        print_menu_interactive();

        // Gather user input

        // Execute on user command
        printf("Hello, World!\n");

        // If command is quit, then set run to false.
        interactive_mode = false;
    }

    // Handle batch case.

    // Handle single file case.

    // Shutdown the program.
    return 0;
}

// The 4 libraries we are allowed.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "datatypes/bool.h"
#include "datatypes/result.h"

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

    char encryption_key[XOR_KEY_SIZE];

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
            strcpy_s(encryption_key, 256, argv[i + 1]);
            i++;
        }
        if(strcmp(argv[i], "--decrypt") == 0 || strcmp(argv[i], "-D") == 0) {
            decrypt_mode = true;
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

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
#include "datatypes/directory_tree.h"

#include "bmp.h"
#include "encryption.h"
#include "input.h"
#include "rle.h"

#define RUNTIME_DEBUG

/*int main(int argc, char* argv[]) {
    // Initialize program
    bool interactive_mode = false;
    bool encrypt_mode = false;
    bool decrypt_mode = false;
    bool compress_mode = false;
    bool decompress_mode = false;
    bool input_file_present = false;
    bool encryption_key_present = false;
    bool ignore_nonfatal = false;

    char input_file_name[256];

    unsigned int *encryption_key = malloc(sizeof(unsigned int));

    // Process arguments
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--interactive") == 0
            || strcmp(argv[i], "-I") == 0) {
            interactive_mode = true;
        }
        if (strcmp(argv[i], "--encrypt") == 0 || strcmp(argv[i], "-E") == 0)
        {
            encrypt_mode = true;
            i++;
        }
        if (strcmp(argv[i], "--decrypt") == 0 || strcmp(argv[i], "-D") == 0)
        {
            decrypt_mode = true;
        }
        if (strcmp(argv[i], "--key") == 0 || strcmp(argv[i], "-K") == 0)
        {
            memcpy(encryption_key, argv[i + 1], 4);
            encryption_key_present = true;
        }
        if (strcmp(argv[i], "--password") == 0 || strcmp(argv[i], "-P") == 0)
        {
            *encryption_key = fnv1a_hash(argv[i + 1]);
        }
        if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-I") == 0)
        {
            strcpy(input_file_name, argv[i + 1]);
            input_file_present = true;
            i++;
        }
        if (strcmp(argv[i], "--ignore-nonfatal") == 0)
        {
            ignore_nonfatal = true;
        }
        if (strcmp(argv[i], "--force-compress") == 0
            || strcmp(argv[i], "-C") == 0)
        {
            compress_mode = true;
        }
        if (strcmp(argv[i], "--force-decompress") == 0
            || strcmp(argv[i], "-B") == 0)
        {
            decompress_mode = true;
        }
    }

    if (compress_mode && decompress_mode)
    {
        printf("The same file can not be both compressed and compressed"
               " at the same time.\n");
        return 0;
    }

    if (!interactive_mode && !encrypt_mode && !decrypt_mode)
    {
        print_menu_help();
        return 0;
    }

    if (!interactive_mode && !input_file_present)
    {
        printf("Please provide a input file using --input (-I) when in"
               " command line.\n");
        return 0;
    }

    if (interactive_mode && (encrypt_mode || decompress_mode || compress_mode
                             || decompress_mode))
    {
        printf("The program can not run in both interactive and command"
               " line mode.");
        return 0;
    }

    // If in interactive mode, Enter the event loop
    while (interactive_mode)
    {
        // Print menu
        print_menu_interactive();

        // Gather user input
        result_t input_result = input_number(1, 6, "Please"
                                                   " select an option in the"
                                                   " range 1.6");
        i8_t* selected_option = input_result.data;

        // Execute on user command
        if (*selected_option == 1)
        // Encrypt
        {

        }
        else if (*selected_option == 2)
        // Decrypt
        {

        }
        else if (*selected_option == 3)
        // Compress
        {

        }
        else if (*selected_option == 4)
        // Decompress
        {

        }
        else if (*selected_option == 5)
        // Info
        {

        }
        else if (*selected_option == 6)
        // Quit
        {
            interactive_mode = false;
        }
    }

    //---- Handle command line case.
    // Note if ignore nonfatal is being used.
    if (ignore_nonfatal)
    {
        printf("Ignoring non-fatal BMP file format errors.\n");
    }

    // Handle encryption
    if (encrypt_mode)
    {
        FILE *input_file;

        #ifdef RUNTIME_DEBUG
        printf("Attempting file read from %s.\n", input_file_name);
        #endif

        // Use file_read() eventually... but for now.
        input_file = fopen(input_file_name, "r");
        if (input_file == NULL)
        {
            printf("An error has occurred reading the file.\n");
            return 0;
        }

        // TEMPORARY FOR TESTING
        option_t key;
        if (encryption_key_present)
        {
            key.present = true;
            key.data = encryption_key;
        }
        else
        {
            key.present = false;
            key.data = NULL;
        }

        // Convert to BMP.
        result_t bmp_result = bmp_from_file(input_file, key, !ignore_nonfatal);
        BMP_t *bmp;

        if (bmp_result.ok)
        {
            bmp = bmp_result.data;
            printf("Successfully read bmp with width: %u, height: %d.\n",
                   bmp->imageHeader.width, bmp->imageHeader.height);
        } else
        {
            printf("An error has occurred reading the BMP Data:\n"
                   "%s.\n", (char *) bmp_result.data);
            return 0;
        }

        // After all is done, make sure to free BMP.
        bmp_destructor(bmp);
    }
    // Handle Decryption
    else if (decrypt_mode)
    {

    }
    // Handle compression
    else if (compress_mode)
    {

    }
    // Handle decompression
    else if (decompress_mode)
    {

    }

    // Shutdown the program.
    free(encryption_key);
    return 0;
}*/

// Test Main
#include "datatypes/directory_tree.h"

int main() {
    char* text = malloc(PATH_MAX);
    strcpy(text, "..\\src");
    directory_t root_dir = directory_tree_new_from_dir_path(text);
}
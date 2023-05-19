// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// ----------------------------Compile Instructions-----------------------------
//
// -----------------------------------main.c------------------------------------

// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Other Includes
#include "datatypes/bool.h"
#include "datatypes/result.h"
#include "datatypes/option.h"
#include "datatypes/directory_tree.h"

#include "bmp.h"
#include "encryption.h"
#include "input.h"

// Static Defines
#define RUNTIME_DEBUG

//------------------------------------------------------------------------------
// Private Function Declarations
//------------------------------------------------------------------------------
void encrypt_file(char* input_name, char* output_name, u32_t* key, bool strict_verify, bool compress);
void decrypt_file(char* input_name, char* output_name, u32_t* key, bool strict_verify, bool compress);
void compress_file(char* input_name, char* output_name, bool strict_verify);
void decompress_file(char* input_name, char* output_name, bool strict_verify);

//------------------------------------------------------------------------------
// Begin Main
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    // Program Operation State
    bool interactive_mode = false;
    bool encrypt_mode = false;
    bool decrypt_mode = false;
    bool compress_mode = false;
    bool decompress_mode = false;
    bool input_file_present = false;
    bool encryption_key_present = false;
    bool ignore_nonfatal = false;

    // Common Mode Operation Variables
    char input_file_name[PATH_MAX];
    char output_file_name[PATH_MAX];
    FILE *input_file = NULL;
    FILE* output_file = NULL;
    BMP_t* bmp;

    // Interactive Mode Operation Variables
    i32_t selected_option;
    char password[PATH_MAX];

    // Encryption Mode Operation Variables
    u32_t *encryption_key = malloc(sizeof(u32_t));

    //--------------------------------------------------------------------------
    // Process command line arguments
    //--------------------------------------------------------------------------
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
            *encryption_key = fnv1a_hash(argv[i + 1],
                                         strlen(argv[i+1]));
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

    // A key is required if in encrypt or decrypt mode.
    if (!encryption_key_present && (encrypt_mode || decrypt_mode))
    {
        printf("An encryption key must be specified when using encrypt"
               " or decrypt mode.\n");
        return 0;
    }

    // Check for errors in the command line arguments
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

    //--------------------------------------------------------------------------
    // Interactive Mode
    //--------------------------------------------------------------------------
    while (interactive_mode)
    {
        // Print menu
        print_menu_interactive();

        // Gather user input
        selected_option = input_number(1, 6,"Please select"
                                            " an option in the range 1 to 6");

        // Execute on user command
        if (selected_option == 1)
        // Encrypt
        {
            // Input file
            printf("Please enter file name to encrypt > ");
            input_string(input_file_name, PATH_MAX);

            // Strict verification
            printf("Would you like to enable strict verification of the"
                   "incoming bmp file? > ");
            ignore_nonfatal = input_bool();

            // Encryption type
            char choice[2];
            bool valid = false;

            while (!valid)
            {
                printf("Would you like to use a password [p] integer [i]"
                       " key? > ");
                input_string(choice, 1);

                if (choice[0] == 'p')
                {
                    // Encryption password
                    printf("Please enter encryption password > ");
                    input_string(password, PATH_MAX);
                    *encryption_key = fnv1a_hash(password, strlen(password));
                    valid = true;
                }
                else if (choice[0] == 'i')
                {
                    // Encryption integer
                    printf("Please enter encryption integer [i32] > ");
                    *encryption_key = (u32_t) (input_number(MIN_i32, MAX_i32, "") + MAX_i32);
                    valid = true;
                }
                else {
                    printf("Please select a valid option.");
                }
            }

            // Compress?
            printf("Would you also like to compress if possible? > ");
            bool compress = input_bool();

            // Output file
            printf("Please enter output file name > ");
            input_string(output_file_name, PATH_MAX);

            encrypt_file(input_file_name, output_file_name,
                         encryption_key, !ignore_nonfatal, compress);
        }
        else if (selected_option == 2)
        // Decrypt
        {

        }
        else if (selected_option == 3)
        // Compress
        {

        }
        else if (selected_option == 4)
        // Decompress
        {

        }
        else if (selected_option == 5)
        // Info
        {

        }
        else if (selected_option == 6)
        // Quit
        {
            interactive_mode = false;
        }
    }

    //--------------------------------------------------------------------------
    // Command Line Mode
    //--------------------------------------------------------------------------
    // Note if ignore nonfatal is being used.
    if (ignore_nonfatal)
    {
        printf("Ignoring non-fatal BMP file format errors.\n");
    }

    // Handle encryption
    if (encrypt_mode)
    {
        #ifdef RUNTIME_DEBUG
        printf("Attempting file read from %s.\n", input_file_name);
        #endif

        file_read(input_file, input_file_name);

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

    //--------------------------------------------------------------------------
    // Program Shutdown
    //--------------------------------------------------------------------------
    free(encryption_key);
    return 0;
}

//------------------------------------------------------------------------------
// Private Function Definitions
//------------------------------------------------------------------------------

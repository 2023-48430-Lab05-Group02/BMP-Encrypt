// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// -----------------------------------int.c-------------------------------------

// Standard Library Includes
#include <stdlib.h>
#include <string.h>

// Public API Includes
#include "int.h"

// Other Includes
#include "input.h"
#include "encryption.h"

#include "main.h"

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void main_interactive() {
    // Variables
    bool interactive = true;
    bool ignore_nonfatal = false;
    i32_t selected_option = -1;

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    char password[PATH_MAX];
    u32_t encryption_key;

    while (interactive)
    {
        // Print menu
        print_menu_interactive();

        // Gather user input
        selected_option = input_number(1, 7,"Please select"
                                            " an option in the range 1 to 7.");

        // Execute on user command
        switch (selected_option)
        {
            case 1: // Encrypt
            {
                // Input file
                printf("Please enter file name to encrypt >");
                input_file_read(input_file);

                // Strict verification
                printf("Would you like to enable strict verification of the"
                       " incoming bmp file? >");
                ignore_nonfatal = input_bool();

                // Encryption Key type
                char choice[2];
                bool valid = false;

                while (!valid)
                {
                    printf("Would you like to use a password [p] integer [i]"
                           " key? >");
                    input_string(choice, 1);

                    if (choice[0] == 'p')
                    {
                        // Encryption password
                        printf("Please enter encryption password >");
                        input_string(password, PATH_MAX);
                        encryption_key = fnv1a_hash(password, strlen(password));
                        valid = true;
                    }
                    else if (choice[0] == 'i')
                    {
                        // Encryption integer
                        printf("Please enter encryption integer [i32] >");
                        encryption_key = (u32_t) (input_number(MIN_i32, MAX_i32, "") + MAX_i32);
                        valid = true;
                    }
                    else {
                        printf("Please select a valid option.");
                    }
                }

                // Compress?
                printf("Would you also like to compress if possible? >");
                bool compress = input_bool();

                // Output file
                printf("Please enter output file name >");
                input_file_write(output_file);

                encrypt_file(input_file, output_file, encryption_key,
                             !ignore_nonfatal, compress);
            }
                break;
            case 2: // Decrypt
                break;
            case 3: // Compress
                break;
            case 4: // Decompress
                break;
            case 5: // Info
                break;
            case 6: // Force non-fatal mode change.
                ignore_nonfatal = !ignore_nonfatal;

                if (ignore_nonfatal)
                {
                    printf("Now ignoring non-fatal errors when reading or"
                           " writing BMP files.\n\n");
                }
                else if (!ignore_nonfatal)
                {
                    printf("No longer ignoring non-fatal errors when reading"
                           " or writing BMP files.\n\n");
                }
                break;
            case 7: // Quit
                interactive = false;
                printf("Quitting BMP-Encrypt!\n");
                break;
            default: // How did we get here?
                exit(1);
                break;
        }
    }
}

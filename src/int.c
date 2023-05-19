// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// -----------------------------------int.c-------------------------------------

// Public API Includes
#include "int.h"

// Other Includes
#include "input.h"

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
result_t main_interactive() {
    // Variables
    result_t result;
    bool interactive = true;


    while (interactive)
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
}
// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// -----------------------------------main.c------------------------------------

// Standard Library Includes
#include <stdio.h>
#include <string.h>

// Public API Includes
#include "main.h"

// Other Includes
#include "datatypes/bool.h"

#include "encryption.h"
#include "input.h"
#include "int.h"
#include "cmd.h"
#include "bmp.h"

// Static Defines
#define RUNTIME_DEBUG

//------------------------------------------------------------------------------
// Begin Main
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    ProgramState_t state = {
      false,
      false,
      false,
      false,
      false,
      false,
      false,

      false,
      "",

      0,
      false,
    };

    //--------------------------------------------------------------------------
    // Process command line arguments
    //--------------------------------------------------------------------------
    /*
     * Short Arguments | Long Arguments:
     *  -B : Batch : --batch
     *  -I : Interactive : --interactive
     *  -E : Encrypt : --encrypt
     *  -D : Decrypt : --decrypt
     *  -K : Key : --key
     *  -P : Password : --password
     *  -I : Input file : --input
     *  -FF: Ignore non-fatal: --force-nonfatal
     *  -FC: Force compress output file if possible : --force-compress
     *  -FD: Force decompress output file if possible --force-decompress
     */
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--batch") == 0
            || strcmp(argv[i], "-B") == 0)
        {
            state.batch_mode = true;
        }
        if (strcmp(argv[i], "--interactive") == 0
            || strcmp(argv[i], "-I") == 0) {
            state.interactive_mode = true;
        }
        if (strcmp(argv[i], "--encrypt") == 0 || strcmp(argv[i], "-E") == 0)
        {
            state.encrypt_mode = true;
        }
        if (strcmp(argv[i], "--decrypt") == 0 || strcmp(argv[i], "-D") == 0)
        {
            state.decrypt_mode = true;
        }
        if (strcmp(argv[i], "--key") == 0 || strcmp(argv[i], "-K") == 0)
        {
            state.encryption_key = (u32_t) atoi(argv[i+1]) + MAX_i32;
            //memcpy(&state.encryption_key, argv[i + 1], 4);
            state.encryption_key_present = true;
            i++;
        }
        if (strcmp(argv[i], "--password") == 0 || strcmp(argv[i], "-P") == 0)
        {
            state.encryption_key = fnv1a_hash(argv[i + 1],
                                         strlen(argv[i+1]));
            state.encryption_key_present = true;
            i++;
        }
        if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-I") == 0)
        {
            strcpy(state.input_file_name, argv[i + 1]);
            state.input_file_present = true;
            i++;
        }
        if (strcmp(argv[i], "--force-nonfatal") == 0
            || strcmp(argv[i], "-FF") == 0)
        {
            state.force_nonfatal_mode = true;
        }
        if (strcmp(argv[i], "--force-compress") == 0
            || strcmp(argv[i], "-FC") == 0)
        {
            state.compress_mode = true;
        }
        if (strcmp(argv[i], "--force-decompress") == 0
            || strcmp(argv[i], "-FD") == 0)
        {
            state.decompress_mode = true;
        }
    }

    //--------------------------------------------------------------------------
    // Handle error conditions in arguments.
    //--------------------------------------------------------------------------
    // A key is required if in encrypt or decrypt mode.
    if (!state.encryption_key_present
        && (state.encrypt_mode || state.decrypt_mode))
    {
        printf("An encryption key must be specified when using encrypt"
               " or decrypt mode.\n");
        return 0;
    }

    // No compress and decompress
    if (state.compress_mode && state.decompress_mode)
    {
        printf("The same file can not be both compressed and compressed"
               " at the same time.\n");
        return 0;
    }

    //
    if (!state.interactive_mode && !state.encrypt_mode && !state.decrypt_mode
        && !state.compress_mode && !state.decompress_mode && !state.batch_mode)
    {
        print_menu_help();
        return 0;
    }

    if (!state.interactive_mode && !state.input_file_present)
    {
        printf("Please provide a input file using --input (-I) when in"
               " command line.\n");
        return 0;
    }

    if (state.interactive_mode && (state.encrypt_mode || state.decrypt_mode
        || state.compress_mode || state.decompress_mode || state.batch_mode))
    {
        printf("The program can not run in both interactive and command"
               " line mode.");
        return 0;
    }
    //--------------------------------------------------------------------------
    // Program Mode Switcher
    //--------------------------------------------------------------------------
    if (state.interactive_mode)
    {
        main_interactive();
    }
    else if (state.batch_mode)
    {
        main_batch(state);
    }
    else
    {
        main_single(state);
    }

    //--------------------------------------------------------------------------
    // Program Shutdown
    //--------------------------------------------------------------------------
    return 0;
}

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
void encrypt_file(FILE* input, FILE* output, u32_t key, bool strict_verify, bool compress) {
    BMP_t* bmp;
    option_t key_option = {true, &key};
    option_t no_key_option = {false, NULL};
    result_t input_result = bmp_from_file(input, no_key_option, strict_verify); /*gets bmp.c to open file*/
    if(!input_result.ok) /*checks input is ok*/
    {
        printf("An error occurred decoding BMP Data: %s", (char*) input_result.data);
        exit(1);
    }

    bmp = input_result.data;
    result_t output_result = bmp_to_file(output, bmp, key_option, compress); /*gets bmp.c to creat new file, encrypts in process*/
    if(!output_result.ok) /*checks output is ok*/
    {
        printf("An error occurred encoding BMP Data: %s", (char*) input_result.data);
        exit(1);
    }

    printf("Completed BMP encrypt.");
}
void decrypt_file(FILE* input, FILE* output, u32_t key, bool strict_verify, bool compress) {
    BMP_t* bmp;
    option_t key_option = {true, &key};
    option_t no_key_option = {false, NULL};
    result_t input_result = bmp_from_file(input, key_option, strict_verify); /*gets bmp.c to open file, decrypts in process*/
    if(!input_result.ok) /*checks input is ok*/
    {
        printf("An error occurred decoding BMP Data: %s", (char*) input_result.data); /*gets bmp.c to creat new file*/
        exit(1);
    }

    bmp = input_result.data;
    result_t output_result = bmp_to_file(output, bmp, no_key_option, compress); /*gets bmp.c to creat new file*/
    if(!output_result.ok) /*checks output is ok*/
    {
        printf("An error occurred encoding BMP Data: %s", (char*) input_result.data);
        exit(1);
    }

    printf("Completed BMP decrypt.");

}
void compress_file(FILE* input, FILE* output, bool strict_verify) {
    BMP_t* bmp;
    option_t no_key_option = {false, NULL};
    result_t input_result = bmp_from_file(input, no_key_option, strict_verify); /*gets bmp.c to open file*/
    if(!input_result.ok) /*checks input is ok*/
    {
        printf("An error occurred decoding BMP Data: %s", (char*) input_result.data);
        exit(1);
    }
    bmp = input_result.data;
    result_t output_result = bmp_to_file(output, bmp, no_key_option, true); /*gets bmp.c to creat new file, compresses in process*/
    if(!output_result.ok) /*checks output is ok*/
    {
        printf("An error occurred encoding BMP Data: %s", (char*) input_result.data);
        exit(1);
    }

    printf("Completed BMP compress.");
}
void decompress_file(FILE* input, FILE* output, bool strict_verify) {
    BMP_t* bmp;
    option_t no_key_option = {false, NULL};
    result_t input_result = bmp_from_file(input, no_key_option, strict_verify); /*gets bmp.c to open file, decompresses in process*/
    if(!input_result.ok) /*checks input is ok*/
    {
        printf("An error occurred decoding BMP Data: %s", (char*) input_result.data);
        exit(1);
    }
    bmp = input_result.data;
    result_t output_result = bmp_to_file(output, bmp, no_key_option, false); /*gets bmp.c to creat new file*/
    if(!output_result.ok) /*checks output is ok*/
    {
        printf("An error occurred encoding BMP Data: %s", (char*) input_result.data);
        exit(1);
    }

    printf("Completed BMP decompress.");

}

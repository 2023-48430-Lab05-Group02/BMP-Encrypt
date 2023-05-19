// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "input.h"

void print_menu_help() {
    printf("Help Test");
    printf("This program is in an image encryption System using bmp files\n");
}
void print_menu_interactive() {
    printf("Interactive Test");
    printf("Which action would you like to perform:\n");
    printf("1. Encrypt\n" 
            "2. Decrypt\n" 
            "3. Compress\n"
            "4. Decompress\n"
            "5. Info \n"
	        "6. Quit \n");
    printf("Select your choice>\n");
}

void print_unsigned_int_binary(unsigned int num) {
    for (int i = sizeof(unsigned int) * 8 - 1; i >= 0; i--)
        printf("%d", (num >> i) & 1);
}

result_t input_number(int min, int max, char error_message[]) {
    	int choice; 
        do{
	print_menu_interactive();
		scanf("%d", & choice);
	
	/* When input is outside of the guidelindes it will print warning message */
        if (choice < 1 || choice > 6)
        {
            printf("Invalid choice. Please select again.");
        }
        else
        {
            switch (choice) {
            case 1:
                Encrypt();
                break;
            case 2:
                Decrypt();
                break;
            case 3:
                Compress();
                break;
            case 4:
                Decompress();
                break;
            case 5:
                Info();
                break;
            }
        }
	} while (choice != 6);
    
    return 0;

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
    FILE* file
    file = fopen(char name [], "r");

// Print some text if the file does not exist
if(file == NULL) {
  printf("Error! File is not valid.");
}

// process the file contents
fclose(file);
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}
result_t file_write(char name[]) {
    FILE* file = fopen(char name[], "wb");
    if (file == NULL) {
        printf("Error! Failed to open the file for writing.\n");
        return;
    }

    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

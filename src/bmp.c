// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bmp.h"

#define RUNTIME_DEBUG

result_t bmp_from_file(FILE* input_file) {
    // Constants/Variables
    char bmp_text[] = {'B', 'M'};
    BMP_t* bmp = malloc(sizeof(BMP_t));
    result_t result;

    // Read the file header
    fread(&bmp->fileHeader, sizeof(BMPFileHeader_t), 1, input_file);

    #ifdef RUNTIME_DEBUG
    printf("File header type is %c%c.\n", bmp->fileHeader.type[0], bmp->fileHeader.type[1]);
    #endif

    // Verify the file is the correct type.
    if(memcmp(bmp->fileHeader.type, bmp_text, 2) != 0) {
        result.ok = false;
        result.data = "FILETYPE NOT BM";
        return result;
    }

    #ifdef RUNTIME_DEBUG
    printf("BMP Header Data: size %u, rsv1 %u, rsv2 %u, offset %u.\n",
           bmp->fileHeader.size, bmp->fileHeader.reserved1,
           bmp->fileHeader.reserved2, bmp->fileHeader.pixelOffset);
    #endif

    // Next read the BMP Image Header data.
    fread(&bmp->imageHeader, sizeof(BMPImageHeader_t), 1, input_file);

    result.ok = true;
    result.data = bmp;
    return result;
}
result_t bmp_to_file(FILE* output_file, BMP_t* bmp) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

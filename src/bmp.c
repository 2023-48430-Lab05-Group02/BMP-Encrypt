// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "bmp.h"

#define RUNTIME_DEBUG

result_t bmp_from_file(FILE* input_file) {
    // Constants/Variables
    char bmp_text[] = {'B', 'M'};
    BMP_t* bmp = malloc(sizeof(BMP_t));

    // Output variable - Assume result is false until proven true.
    result_t result;
    result.ok = false;

    // Read the file header
    fread(&bmp->fileHeader, sizeof(BMPFileHeader_t), 1, input_file);

    #ifdef RUNTIME_DEBUG
    printf("File header type is %c%c.\n", bmp->fileHeader.type[0], bmp->fileHeader.type[1]);
    #endif

    // Verify the file is the correct type.
    if(memcmp(bmp->fileHeader.type, bmp_text, 2) != 0) {
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

    // Check that we are dealing with the expected Windows Version 3 Header.
    if (bmp->imageHeader.size != 40) {
        result.data = "HEADER NOT WIN3 FORMAT";
        return result;
    }

    // Verify the image parameters as valid.
    if (bmp->imageHeader.height <= 0 || bmp->imageHeader.width <= 0) {
        result.data = "IMAGE SIZE IMPOSSIBLE";
        return result;
    }

    // Check if we are dealing with an unsupported multi-plane BMP.
    if (bmp->imageHeader.planes != 1) {
        result.data = "UNSUPPORTED MULTI-PLANE BMP";
        return result;
    }

    // Determine if a color or mask table will be attached.
    if (bmp->imageHeader.bitDepth <= 8) {
        bmp->bitMaskTable.present = false;
        bmp->bitMaskTable.data = NULL;

        bmp->colorTable.data = malloc(sizeof(BMPColorTableHeader_t));
        bmp->colorTable.present = true;
        fread(&((BMPColorTableHeader_t*)bmp->colorTable.data)->colorData, sizeof(unsigned int), bmp->imageHeader.clrsUsed, input_file);

        #ifdef RUNTIME_DEBUG
        printf("Read Color Table with %u colors.\n", bmp->imageHeader.clrsUsed);
        #endif
    }
    else if (bmp->imageHeader.bitDepth == 16 || bmp->imageHeader.bitDepth == 32) {
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.data = malloc(sizeof(BMPMaskTableHeader_t));
        bmp->bitMaskTable.present = true;
        fread(&bmp->bitMaskTable.data, sizeof(BMPMaskTableHeader_t), 1, input_file);

        #ifdef RUNTIME_DEBUG
        printf("Read Mask Table | R: %u, G: %u, B: %u.\n",
               ((BMPMaskTableHeader_t*)bmp->colorTable.data)->red_mask,
               ((BMPMaskTableHeader_t*)bmp->colorTable.data)->green_mask,
               ((BMPMaskTableHeader_t*)bmp->colorTable.data)->blue_mask);
        #endif
    }
    else {
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.present = false;
        bmp->bitMaskTable.data = NULL;
    }

    // Finally read the pixel data.
    unsigned int pixel_count = bmp->imageHeader.width * bmp->imageHeader.height;
    unsigned int bits = pixel_count * bmp->imageHeader.bitDepth;
    double bytes = (double) bits / 8;
    unsigned int bytes_nearest = (unsigned int) ceil(bytes);
    bmp->pixelData = malloc(bytes_nearest);
    fread(bmp->pixelData, sizeof(char), bytes_nearest, input_file);

    #ifdef RUNTIME_DEBUG
    printf("First 32 bytes: \n");
    for (int i=0; i < 32; i++) {
        printf("%#010x ", bmp->pixelData[i]);
    }
    #endif

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

// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "bmp.h"

#include "input.h"
#include "rle.h"

#define RUNTIME_DEBUG

result_t bmp_from_file(FILE* input_file) {
    // Constants/Variables
    char bmp_text[] = {'B', 'M'};
    BMP_t* bmp = malloc(sizeof(BMP_t));
    unsigned int file_length;

    // Output variable - Assume result is false until proven true.
    result_t result;
    result.ok = false;

    // Get the file length.
    fseek(input_file, 0L, SEEK_END);
    file_length = (unsigned int) ftell(input_file);
    rewind(input_file);

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

    // Check if the file is the correct length specified.
    if (file_length != bmp->fileHeader.size) {
        result.data = "FILE INVALID SIZE HEADER";
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
        result.data = "HEADER NOT WIN NT3 FORMAT";
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
    else if ((bmp->imageHeader.bitDepth == 16 || bmp->imageHeader.bitDepth == 32) && bmp->imageHeader.compression == 3) {
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.data = malloc(sizeof(BMPMaskTableHeader_t));
        bmp->bitMaskTable.present = true;
        fread(bmp->bitMaskTable.data, sizeof(BMPMaskTableHeader_t), 1, input_file);

        #ifdef RUNTIME_DEBUG
        printf("Read Mask Table | R, G, B.\n");
        print_unsigned_int_binary(((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->red_mask);
        printf("\n");
        print_unsigned_int_binary(((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->green_mask);
        printf("\n");
        print_unsigned_int_binary(((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->blue_mask);
        printf("\n");
        #endif
    }
    else {
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.present = false;
        bmp->bitMaskTable.data = NULL;
    }

    // Verify file pointer position sync.
    if (bmp->fileHeader.pixelOffset != (unsigned int) ftell(input_file)) {
        result.data = "PIXEL DATA OFFSET MISMATCH";
        return result;
    }

    // Determine the final number of bytes of data we should be receiving
    unsigned int pixel_count = bmp->imageHeader.width * bmp->imageHeader.height;
    unsigned int bits = pixel_count * bmp->imageHeader.bitDepth;
    double bytes = (double) bits / 8;
    unsigned int bytes_nearest = (unsigned int) ceil(bytes);

    // If the data is RLE8/RLE4 encoded, read and decode the data.
    if (bmp->imageHeader.compression == 1 || bmp->imageHeader.compression == 2) {
        char *buffer = malloc(bmp->imageHeader.imageSize);
        result_t rle_result;
        if (bmp->imageHeader.compression == 1) {
            // RLE8
            rle_result = rl8_decode(&buffer, (int) bmp->imageHeader.imageSize);
        } else if (bmp->imageHeader.compression == 2) {
            //RLE 4
            rle_result = rl4_decode(&buffer, (int) bmp->imageHeader.imageSize);
        }
        if (rle_result.ok == false) {
            char* error = malloc(256);
            strcpy(error, "RLE DECODE FAILURE: ");
            result.data = strcat(error, rle_result.data);
            return result;
        } else {
            bmp->pixelData = rle_result.data;
        }
    }
    // Otherwise read the straight data from the file.
    else {
        bmp->pixelData = malloc(bytes_nearest);
        fread(bmp->pixelData, sizeof(char), bytes_nearest, input_file);
    }

    #ifdef RUNTIME_DEBUG
    printf("First 8 bytes: \n");
    for (int i=0; i < 8; i++) {
        printf("%#010x ", bmp->pixelData[i]);
    }
    printf("\n");
    #endif

    // Verify the pixels are not outside the range of the palette.
    if (bmp->colorTable.present) {
        for (unsigned int i = bmp->imageHeader.height * bmp->imageHeader.width; i > 0; i--) {
            if (bmp->pixelData[i] > (char) bmp->imageHeader.clrsUsed) {
                result.data = "PIXEL COLOR OUTSIDE COLOR TABLE";
                return result;
            }
        }
    }

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

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
#include "encryption.h"

#define RUNTIME_DEBUG

result_t bmp_from_file(FILE* input_file, option_t key) {
    // Constants/Variables
    char bmp_text[] = {'B', 'M'};
    BMP_t* bmp = malloc(sizeof(BMP_t));
    unsigned int file_length;
    char* bmp_data;
    unsigned int bmp_data_position = 0;

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
        result.data = "FILE INVALID SIZE HEADER OR FILE SIZE INVALID";
        return result;
    }

    // Check if the file is encrypted.
    if (bmp->fileHeader.reserved1 == 1) {
        if(key.present == false) {
            result.data ="NO ENCRYPTION KEY PROVIDED";
            return result;
        }

        char* decrypt_buffer = malloc(file_length - 14);
        fread(decrypt_buffer, sizeof(char), file_length - 14, input_file);

        result_t xor_result = xor_decrypt(decrypt_buffer, (int) (file_length - 14), key.data);

        // Make sure to free the decrypt buffer.
        free(decrypt_buffer);

        if (xor_result.ok == false) {
            char* error = malloc(256);
            strcpy(error, "XOR DECRYPT FAILURE: ");
            result.data = strcat(error, xor_result.data);
            return result;
        }

        bmp_data = xor_result.data;
    } else {
        bmp_data = malloc(file_length - 14);
        fread(bmp_data, sizeof(char), file_length - 14, input_file);
    }

    // Check if reserved values are out of spec.
    if (bmp->fileHeader.reserved2 != 0 || bmp->fileHeader.reserved1 > 1) {
        result.data = "RESERVED VALUES OUT OF SPEC";
        return result;
    }

    #ifdef RUNTIME_DEBUG
    printf("BMP Header Data: size %u, rsv1 %u, rsv2 %u, offset %u.\n",
           bmp->fileHeader.size, bmp->fileHeader.reserved1,
           bmp->fileHeader.reserved2, bmp->fileHeader.pixelOffset);
    #endif

    // Next read the BMP Image Header data.
    memcpy(&bmp->imageHeader, bmp_data, sizeof(BMPImageHeader_t));
    bmp_data_position += sizeof(BMPImageHeader_t);

    #ifdef RUNTIME_DEBUG
    printf("Image Header:\nHeight: %d, Width: %u, Compression: %u\n"
           "X Pixels Per Meter: %u, Y Pixels Per Meter: %u\n"
           "Bit Count: %u, Header Size: %u, Planes: %u\n"
           "Colors used: %u, Colors Important: %u, Image Size: %u\n",
           bmp->imageHeader.height, bmp->imageHeader.width,
           bmp->imageHeader.compression, bmp->imageHeader.xPixelsPerMeter,
           bmp->imageHeader.yPixelsPerMeter, bmp->imageHeader.bitDepth,
           bmp->imageHeader.size, bmp->imageHeader.planes,
           bmp->imageHeader.clrsUsed, bmp->imageHeader.clrsImportant,
           bmp->imageHeader.imageSize);
    #endif

    // Check that we are dealing with the expected Windows Version 3 Header.
    if (bmp->imageHeader.size != 40) {
        result.data = "HEADER NOT WIN NT3 FORMAT";
        return result;
    }

    // Verify the image parameters as valid.
    if (bmp->imageHeader.width <= 0) {
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

        // Handle cases of color table (assumed) sizes.
        if(bmp->imageHeader.clrsUsed == 0) {
            bmp->imageHeader.clrsUsed = (unsigned int) pow(2.0, (double) bmp->imageHeader.bitDepth);
            #ifdef RUNTIME_DEBUG
            printf("Assumed color table due to 0 used colors in source file.");
            #endif
        }

        memcpy(&((BMPColorTableHeader_t*)bmp->colorTable.data)->colorData, bmp_data + bmp_data_position, bmp->imageHeader.clrsUsed);
        bmp_data_position += (unsigned int) bmp->imageHeader.clrsUsed * (unsigned int) sizeof(unsigned int);

        #ifdef RUNTIME_DEBUG
        printf("Read Color Table with %u colors.\n", bmp->imageHeader.clrsUsed);
        #endif
    }
    else if ((bmp->imageHeader.bitDepth == 16 || bmp->imageHeader.bitDepth == 32) && bmp->imageHeader.compression == 3) {
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.data = malloc(sizeof(BMPMaskTableHeader_t));
        bmp->bitMaskTable.present = true;
        memcpy(bmp->bitMaskTable.data, bmp_data + bmp_data_position, sizeof(BMPMaskTableHeader_t));
        bmp_data_position += sizeof(BMPMaskTableHeader_t);

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
    if (bmp->colorTable.present == false) {
        if (bmp->fileHeader.pixelOffset - 14 != (unsigned int) bmp_data_position) {
            result.data = "PIXEL DATA OFFSET MISMATCH";
            return result;
        }
    } else {
        if ((bmp->fileHeader.pixelOffset - 14 + bmp->imageHeader.clrsUsed * 4) != (unsigned int) bmp_data_position && (bmp->fileHeader.pixelOffset - 14) != (unsigned int) bmp_data_position) {
            result.data = "PIXEL DATA OFFSET MISMATCH WITH COLOR TABLE";
            return result;
        }
    }

    // Determine the final number of bytes of data we should be receiving
    unsigned int pixel_count = bmp->imageHeader.width * (unsigned int) abs(bmp->imageHeader.height);
    unsigned int bits = pixel_count * bmp->imageHeader.bitDepth;
    double bytes = (double) bits / 8;
    unsigned int bytes_nearest = (unsigned int) ceil(bytes);

    // If the data is RLE8/RLE4 encoded, read and decode the data.
    if (bmp->imageHeader.compression == 1 || bmp->imageHeader.compression == 2) {
        char *rle_buffer = malloc(bmp->imageHeader.imageSize);

        memcpy(rle_buffer, bmp_data + bmp_data_position,bmp->imageHeader.imageSize);
        bmp_data_position += bmp->imageHeader.imageSize;

        result_t rle_result;
        if (bmp->imageHeader.compression == 1) {
            // RLE8
            rle_result = rl8_decode(&rle_buffer, (int) bmp->imageHeader.imageSize);
        } else if (bmp->imageHeader.compression == 2) {
            //RLE 4
            rle_result = rl4_decode(&rle_buffer, (int) bmp->imageHeader.imageSize);
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
        memcpy(bmp->pixelData, bmp_data + bmp_data_position, bytes_nearest);
        bmp_data_position += bytes_nearest;
    }

    #ifdef RUNTIME_DEBUG
    printf("First 4 bytes: \n");
    for (int i=0; i < 4; i++) {
        printf("%#010x ", bmp->pixelData[i]);
    }
    printf("\n");
    #endif

    // Verify that at bit depth 2, we have the appropriate monochrome pallet.
    if (bmp->imageHeader.bitDepth == 1 && bmp->imageHeader.clrsUsed != 2) {
        result.data = "COLOR TABLE IS NOT MONOCHROME OR MISSING";
        return result;
    }

    // Verify the pixels are not outside the range of the palette.
    if (bmp->colorTable.present) {
        #ifdef RUNTIME_DEBUG
        printf("Verifying that no pixels are outside the color table. "
               "MAX: %u\n", bmp->imageHeader.clrsUsed);
        #endif
        // Ignore 1 BPP as it is impossible for 1 bit to be out of range.
         if (bmp->imageHeader.bitDepth == 4) {
            for (unsigned int i = bytes_nearest; i > 0; i--) {
                unsigned char upper = bmp->pixelData[i - 1] >> 4;
                unsigned char lower = ((unsigned char) (bmp->pixelData[i - 1] << 4) >> 4);
                if (upper > (char) bmp->imageHeader.clrsUsed || lower > (char) bmp->imageHeader.clrsUsed) {
                    result.data = "PIXEL COLOR OUTSIDE COLOR TABLE";

                    #ifdef RUNTIME_DEBUG
                    printf("Upper Pixel: %u, Lower Pixel: %u at position: %u\n", upper, lower, i);
                    #endif

                    return result;
                }
            }
        } else if (bmp->imageHeader.bitDepth == 8) {
            for (unsigned int i = bytes_nearest; i > 0; i--) {
                if ((unsigned int) bmp->pixelData[i - 1] > bmp->imageHeader.clrsUsed) {
                    result.data = "PIXEL COLOR OUTSIDE COLOR TABLE";

                    #ifdef RUNTIME_DEBUG
                    printf("Pixel data is: %u at position: %u\n", (unsigned int) bmp->pixelData[i - 1], i);
                    #endif

                    return result;
                }
            }
        }
    }

    // Make sure to free the buffers
    free(bmp_data);

    result.ok = true;
    result.data = bmp;
    return result;
}
result_t bmp_to_file(FILE* output_file, BMP_t* bmp, option_t key) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

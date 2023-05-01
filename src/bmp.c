// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

// C Header Includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Public API Includes
#include "bmp.h"

// Other API Includes
#include "input.h"
#include "rle.h"
#include "encryption.h"

// Print verbose debug statements for this module
#define RUNTIME_DEBUG

// Static Defines
#define bmp_file_signature_1 'B'
#define bmp_file_signature_2 'M'

// Private Function Declarations


// Public API Function Definitions
result_t bmp_from_file(FILE* input_file, option_t key, bool strict_verify) {
    // Function Wide Variables
    BMP_t* bmp = malloc(sizeof(BMP_t));
    unsigned int input_file_length;
    // Local bmp_data cache for use in encryption or decryption and safe
    // handling of file contents.
    char* bmp_data;
    unsigned int bmp_data_position = 0;

    // Function Output Variables
    result_t result;
    // Assume the result is not ok until proven to be okay.
    result.ok = false;

    // Get the file's length
    fseek(input_file, 0L, SEEK_END);
    input_file_length = (unsigned int) ftell(input_file);
    rewind(input_file);

    // Read the BMP file header
    fread(&bmp->fileHeader, sizeof(BMPFileHeader_t), 1,
          input_file);

    // DEBUG: Print file magic bytes to stdio.
    #ifdef RUNTIME_DEBUG
    printf("File header type is %c%c.\n", bmp->fileHeader.type[0],
           bmp->fileHeader.type[1]);
    #endif
    // END DEBUG

    // Check if any values in the file header are signed when they shouldn't be.
    if (bmp->fileHeader.size >> 31 == 1 ||
        bmp->fileHeader.reserved1 >> 15 == 1 ||
        bmp->fileHeader.reserved2 >> 15 == 1 ||
        bmp->fileHeader.pixelOffset >> 31 == 1 ) {
        result.data = "INVALID NEGATIVE OR TOO BIG VALUE IN FILE HEADER";
        return result;
    }

    // Verify the file is the correct type using the file signature.
    if(bmp->fileHeader.type[0] != bmp_file_signature_1 ||
       bmp->fileHeader.type[1] != bmp_file_signature_2) {
        result.data = "FILETYPE NOT BM";
        return result;
    }

    // If strictly verifying the file, ensure that the size specified in the
    // file is equal to the actual size of the file. This can make sure that
    // (for example), if the file was copied it wasn't cancelled mid-copy.
    if ((input_file_length != bmp->fileHeader.size) && strict_verify) {
        result.data = "FILE INVALID SIZE HEADER OR FILE SIZE INVALID";
        return result;
    }

    // Handle an encrypted file using our custom encryption standard where
    // the first reserved value of the file header is 1 to indicate encryption.
    if (bmp->fileHeader.reserved1 == 1) {
        // Make sure that a key is provided if the file is encrypted.
        if(key.present == false) {
            result.data ="NO ENCRYPTION KEY PROVIDED";
            return result;
        }

        // Allocate a temporary buffer to hold the data to decrypt. Read the
        // rest of the file into it.
        char* decrypt_buffer = malloc(input_file_length - 14);
        fread(decrypt_buffer, sizeof(char), input_file_length - 14,
              input_file);

        result_t xor_result = xor_decrypt(decrypt_buffer,
                                          (int) (input_file_length - 14),
                                          key.data);

        // Make sure to free the decrypt buffer.
        free(decrypt_buffer);

        // Verify that the XOR decrypt has not returned any errors.
        // Later checks will ensure the decryption has actually succeeded.
        if (xor_result.ok == false) {
            char* error = malloc(256);
            strcpy(error, "XOR DECRYPT FAILURE: ");
            result.data = strcat(error, xor_result.data);
            return result;
        }

        bmp_data = xor_result.data;
    } else {
        // Otherwise if the file is not encrypted, just read it straight into
        // the bmp_data buffer.
        bmp_data = malloc(input_file_length - 14);
        fread(bmp_data, sizeof(char), input_file_length - 14,
              input_file);
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
    if (bmp_data_position + sizeof(BMPImageHeader_t) > input_file_length) {
        result.data = "FILE TOO SMALL FOR BMP IMAGE HEADER";
        return result;
    }
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

    // Check if any values in the image header are signed.
    if (bmp->imageHeader.size >> 31 == 1 ||
        bmp->imageHeader.width >> 31 == 1 ||
        bmp->imageHeader.planes >> 15 == 1 ||
        bmp->imageHeader.bitDepth >> 15 == 1 ||
        bmp->imageHeader.compression >> 31 == 1 ||
        bmp->imageHeader.imageSize >> 31 == 1 ||
        bmp->imageHeader.xPixelsPerMeter >> 31 == 1 ||
        bmp->imageHeader.yPixelsPerMeter >> 31 == 1 ||
        bmp->imageHeader.clrsUsed >> 31 == 1 ||
        bmp->imageHeader.clrsImportant >> 31 == 1) {
        result.data = "INVALID NEGATIVE OR TOO BIG VALUE IN IMAGE HEADER";
        return result;
    }

    // Check that we are dealing with the expected Windows Version 3 Header.
    if (bmp->imageHeader.size != 40) {
        result.data = "HEADER NOT WIN NT3 FORMAT";
        return result;
    }

    // Check if we are dealing with an unsupported multi-plane BMP.
    if (bmp->imageHeader.planes != 1) {
        result.data = "UNSUPPORTED MULTI-PLANE BMP";
        return result;
    }

    // Check color table is smaller than maximum allowed.
    if (bmp->imageHeader.clrsUsed > pow(2.0, bmp->imageHeader.bitDepth)) {
        result.data = "CLRS USED MORE THAN MAXIMUM POSSIBLE FOR BIT DEPTH";
        return result;
    }

    // Check Pixels Per Meter smaller than image. SANITY CHECK.
    if (bmp->imageHeader.xPixelsPerMeter > bmp->imageHeader.width * 1024 + 1024) {
        result.data = "X PIXELS PER METER LARGER THAN IMAGE WIDTH";
        return result;
    }
    if (bmp->imageHeader.yPixelsPerMeter > (unsigned int) abs(bmp->imageHeader.height * 1024 + 1024)) {
        result.data = "Y PIXELS PER METER LARGER THAN IMAGE HEIGHT";
        return result;
    }
    // Also check if Pixels Per meter is 0. SANITY CHECK.
    if ((bmp->imageHeader.yPixelsPerMeter == 0 || bmp->imageHeader.xPixelsPerMeter == 0) && strict_verify) {
        result.data = "PIXELS PER METER ZERO";
        return result;
    }

    // Check if compression is supported.
    if (bmp->imageHeader.compression > 3) {
        result.data = "COMPRESSION UNSUPPORTED";
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
            printf("Assumed color table due to 0 used colors in source file.\n");
            #endif
        }

        if ((bmp_data_position + bmp->imageHeader.clrsUsed * (unsigned int) sizeof(unsigned int)) > input_file_length) {
            result.data = "FILE TOO SMALL FOR BMP COLOR TABLE";
            return result;
        }
        memcpy(&((BMPColorTableHeader_t*)bmp->colorTable.data)->colorData, bmp_data + bmp_data_position, bmp->imageHeader.clrsUsed * (unsigned int) sizeof(unsigned int));
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

        if (bmp_data_position + sizeof(BMPMaskTableHeader_t) > input_file_length) {
            result.data = "FILE TOO SMALL FOR BMP MASK TABLE";
            return result;
        }
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

        // SANITY CHECK - All colors have data? right?
        if ((((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->red_mask == 0 ||
            ((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->green_mask == 0 ||
            ((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->blue_mask == 0) &&
            strict_verify) {
            result.data = "COLOR CHANNEL HAS NO MASK";
            return result;
        }
    }
    else {
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.present = false;
        bmp->bitMaskTable.data = NULL;
    }

    // Check the number of important colors is smaller than the color table.
    if (bmp->imageHeader.clrsImportant > bmp->imageHeader.clrsUsed && bmp->colorTable.present) {
        result.data = "CLRS IMPORTANT MORE THAN COLORS USED";
        return result;
    }

    // Verify file pointer position sync.
    if (bmp->fileHeader.pixelOffset - 14 != (unsigned int) bmp_data_position) {
        result.data = "PIXEL DATA OFFSET MISMATCH";
        return result;
    }

    // Determine the final number of bytes of data we should be receiving
    unsigned int bits_per_row = bmp->imageHeader.width * bmp->imageHeader.bitDepth;
    if (bmp->imageHeader.bitDepth != 32) {
        if (bits_per_row % 32 != 0) {
            unsigned int padding_needed = 32 - (bits_per_row % 32);
            bits_per_row += padding_needed;
        }
    }

    double bits_count = (double) bits_per_row * (double) abs(bmp->imageHeader.height);
    double bytes = bits_count / 8;
    if (bytes > 4294967295) {
        result.data = "IMAGE TOO LARGE";
        return result;
    }
    unsigned int bytes_nearest = (unsigned int) ceil(bytes);

    // Check for uncompressed bitmaps where size does not match expected
    if (bmp->imageHeader.imageSize != 0 && bmp->imageHeader.imageSize != bytes_nearest && bmp->imageHeader.compression == 0) {
        result.data = "PIXEL DATA SIZE DOES NOT MATCH EXPECTED";
        return result;
    }

    // If the data is RLE8/RLE4 encoded, read and decode the data.
    if (bmp->imageHeader.compression == 1 || bmp->imageHeader.compression == 2) {
        char *rle_buffer = malloc(bmp->imageHeader.imageSize);

        if (bmp_data_position + bmp->imageHeader.imageSize > input_file_length) {
            result.data = "FILE TOO SMALL FOR BMP PIXEL DATA COMPRESSED";
            return result;
        }
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

        if (bmp_data_position + bytes_nearest > input_file_length) {
            result.data = "FILE TOO SMALL FOR BMP PIXEL DATA";
            return result;
        }
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

        // Cache this for higher performance.
        unsigned int clrs_used_cache = bmp->imageHeader.clrsUsed;

         if (bmp->imageHeader.bitDepth == 4) {
            for (unsigned int i = bytes_nearest; i > 0; i--) {
                unsigned char prev_pixel = bmp->pixelData[i - 1];
                unsigned char upper = prev_pixel >> 4;
                unsigned char lower = prev_pixel & 0x0F;
                if (upper > clrs_used_cache || lower > clrs_used_cache) {
                    result.data = "PIXEL COLOR OUTSIDE COLOR TABLE";

                    #ifdef RUNTIME_DEBUG
                    printf("Upper Pixel: %u, Lower Pixel: %u at position: %u\n", upper, lower, i);
                    #endif

                    return result;
                }
            }
        } else if (bmp->imageHeader.bitDepth == 8) {
            for (unsigned int i = bytes_nearest; i > 0; i--) {
                if ((unsigned int) bmp->pixelData[i - 1] > clrs_used_cache) {
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

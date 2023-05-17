// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Static Defines
//------------------------------------------------------------------------------
#define bmp_file_signature_1 'B'
#define bmp_file_signature_2 'M'
#define RUNTIME_DEBUG

//------------------------------------------------------------------------------
// Private Structs
//------------------------------------------------------------------------------
typedef struct heapBlock {
    u32_t position;
    u32_t length;
    void* data;
} heapBlock_t;

//------------------------------------------------------------------------------
// Private Function Declarations
//------------------------------------------------------------------------------
/*
 * Reads memory from src.data of quantity size * length to dst.
 * Verifies that src has enough memory remaining and begins reading from
 * the current position in the heap block.
 */
result_t heap_read(void* dst, heapBlock_t* src, u64_t size, u32_t length);
/*
 * Writes memory from src to the heap block in a safe and checked way. Verifies
 * that dst has enough blocks remaining and writes from the current position.
 */
result_t heap_write(heapBlock_t* dst, void* src, u64_t size, u32_t length);

//------------------------------------------------------------------------------
// Public API Function Definitions
//------------------------------------------------------------------------------
result_t bmp_from_file(FILE* input_file, option_t key, bool strict_verify) {
    //-- Function Wide Variables
    BMP_t* bmp = malloc(sizeof(BMP_t));
    u32_t input_file_length;
    result_t read_result;
    u32_t pixel_bytes;

    // Local bmp_data cache for use in encryption or decryption and safe
    // handling of file contents.
    heapBlock_t bmp_raw;

    //-- Function Output Variables
    result_t result;
    // Assume the result is not ok until proven to be okay.
    result.ok = false;

    //-- Processing File: File Header
    // Get the file's length
    fseek(input_file, 0L, SEEK_END);
    input_file_length = (u32_t) ftell(input_file);
    rewind(input_file);

    // Read the BMP file header
    fread(&bmp->fileHeader, sizeof(BMPFileHeader_t),
          1,input_file);

    // DEBUG: Print file magic bytes to stdio.
    #ifdef RUNTIME_DEBUG
    printf("File header type is %c%c.\n", bmp->fileHeader.type[0],
           bmp->fileHeader.type[1]);
    #endif
    // END DEBUG

    // Check if any values in the file header are signed when they shouldn't be.
    if (bmp->fileHeader.size >> 31 == 1
        || bmp->fileHeader.reserved1 >> 15 == 1
        || bmp->fileHeader.reserved2 >> 15 == 1
        || bmp->fileHeader.pixelOffset >> 31 == 1 )
    {
        result.data = "NEGATIVE OR OVERSIZE VALUE IN FILE HEADER";
        return result;
    }

    // Verify the file is the correct type using the file signature.
    if(bmp->fileHeader.type[0] != bmp_file_signature_1
       || bmp->fileHeader.type[1] != bmp_file_signature_2)
    {
        result.data = "FILETYPE NOT BM";
        return result;
    }

    // If strictly verifying the file, ensure that the size specified in the
    // file is equal to the actual size of the file. This can make sure that
    // (for example), if the file was copied it wasn't cancelled mid-copy.
    if ((input_file_length != bmp->fileHeader.size) && strict_verify)
    {
        result.data = "FILE INVALID SIZE HEADER OR FILE SIZE INVALID";
        return result;
    }

    //-- Processing File: Copy Main Data
    // Read the remaining BMP data into a heapBlock in case we need to XOR it.
    bmp_raw.position = 0;
    bmp_raw.length = input_file_length - 14;
    bmp_raw.data = malloc(input_file_length - 14);

    fread(bmp_raw.data, 1,
          input_file_length - 14, input_file);

    //-- Processing File: Decryption
    // Handle an encrypted file using our custom encryption standard where
    // the first reserved value of the file header is 1 to indicate encryption.
    if (bmp->fileHeader.reserved1 == 1)
    {
        // Make sure that a key is provided if the file is encrypted.
        if(key.present == false)
        {
            result.data ="NO ENCRYPTION KEY PROVIDED";
            return result;
        }

        result_t xor_result = xor_decrypt(bmp_raw.data,
                                          bmp_raw.length,
                                          key.data);

        // Verify that the XOR decrypt has not returned any errors.
        // Later checks will ensure the decryption has actually succeeded.
        if (xor_result.ok == false)
        {
            char* error = malloc(256);
            strcpy(error, "XOR DECRYPT FAILURE: ");
            result.data = strcat(error, xor_result.data);
            return result;
        }
    }

    // Check if reserved values are out of spec.
    if ((bmp->fileHeader.reserved2 != 0 || bmp->fileHeader.reserved1 > 1)
        && strict_verify)
    {
        result.data = "RESERVED VALUES OUT OF SPEC";
        return result;
    }

    // DEBUG: Print header data to terminal
    #ifdef RUNTIME_DEBUG
    printf("BMP Header Data: size %u, rsv1 %u, rsv2 %u, offset %u.\n",
           bmp->fileHeader.size, bmp->fileHeader.reserved1,
           bmp->fileHeader.reserved2, bmp->fileHeader.pixelOffset);
    #endif
    // END DEBUG

    //-- Processing File: Image Header
    // Next read the BMP Image Header data.
    read_result = heap_read(&bmp->imageHeader, &bmp_raw,
              sizeof(BMPImageHeader_t), 1);

    // Return error if heap read fails.
    if (read_result.ok == false)
    {
        char* error = malloc(256);
        strcpy(error, "ERROR READING IMAGE HEADER: ");
        result.data = strcat(error, read_result.data);
        return result;
    }

    // DEBUG: Print image header values to terminal.
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
    // END DEBUG

    // Check if any values in the image header are signed.
    if (bmp->imageHeader.size >> 31 == 1
        || bmp->imageHeader.width >> 31 == 1
        || bmp->imageHeader.height >> 31 == 1
        || (bmp->imageHeader.planes >> 15 == 1 && strict_verify)
        || bmp->imageHeader.bitDepth >> 15 == 1
        || bmp->imageHeader.compression >> 31 == 1
        || bmp->imageHeader.imageSize >> 31 == 1
        || (bmp->imageHeader.xPixelsPerMeter >> 31 == 1 && strict_verify)
        || (bmp->imageHeader.yPixelsPerMeter >> 31 == 1 && strict_verify)
        || bmp->imageHeader.clrsUsed >> 31 == 1
        || (bmp->imageHeader.clrsImportant >> 31 == 1 && strict_verify))
    {
        result.data = "INVALID NEGATIVE OR TOO BIG VALUE IN IMAGE HEADER";
        return result;
    }

    // Check that we are dealing with the expected Windows Version 3 Header.
    if (bmp->imageHeader.size != 40)
    {
        result.data = "HEADER NOT WIN NT3 FORMAT";
        return result;
    }

    // Check if the BMP contains multiple "planes". These were never implemented
    // and so should never be anything other than 1. If it is not 1, we can just
    // make an assumption that the number of planes is one.
    if (bmp->imageHeader.planes != 1)
    {
        if (strict_verify) {
            result.data = "UNSUPPORTED MULTI-PLANE BMP";
            return result;
        }
        bmp->imageHeader.planes = 1;
    }

    // Check color table is smaller than maximum allowed.
    if (bmp->imageHeader.clrsUsed > pow(2.0, bmp->imageHeader.bitDepth))
    {
        result.data = "CLRS USED MORE THAN MAXIMUM POSSIBLE FOR BIT DEPTH";
        return result;
    }

    // Check If pixels per meter is way too impossibly large.
    if (strict_verify && bmp->imageHeader.xPixelsPerMeter >
        bmp->imageHeader.width * 1024 + 1024)
    {
        result.data = "X PIXELS PER METER LARGER THAN IMAGE WIDTH";
        return result;
    }
    if (strict_verify && bmp->imageHeader.yPixelsPerMeter >
        (u32_t) abs(bmp->imageHeader.height * 1024 + 1024))
    {
        result.data = "Y PIXELS PER METER LARGER THAN IMAGE HEIGHT";
        return result;
    }

    // Check if pixels per meter is not zero.
    if ((bmp->imageHeader.yPixelsPerMeter == 0
        || bmp->imageHeader.xPixelsPerMeter == 0)
        && strict_verify)
    {
        result.data = "PIXELS PER METER ZERO";
        return result;
    }

    // Check if the type of compression the file uses is supported.
    if (bmp->imageHeader.compression > 3)
    {
        result.data = "COMPRESSION UNSUPPORTED";
        return result;
    }

    //-- Processing File: Color Table or Bit Mask Table (Optional)
    // Determine if a color or mask table should be attached and read it.
    if (bmp->imageHeader.bitDepth <= 8)
    {
        // Make sure it is set that there is mask table.
        bmp->bitMaskTable.present = false;
        bmp->bitMaskTable.data = NULL;

        bmp->colorTable.data = malloc(sizeof(BMPColorTableHeader_t));
        bmp->colorTable.present = true;

        // Handle cases of color table (assumed) sizes.
        if(bmp->imageHeader.clrsUsed == 0)
        {
            bmp->imageHeader.clrsUsed = (u32_t) pow(2.0, (f64_t)
                                        bmp->imageHeader.bitDepth);

            // DEBUG: Note when assumed color tables are in use.
            #ifdef RUNTIME_DEBUG
            printf("Assumed color table due to 0 used colors.\n");
            #endif
            // END DEBUG
        }

        // Read BMP color table from heap.
        heap_read(bmp->colorTable.data, &bmp_raw,
                  sizeof(u32_t), bmp->imageHeader.clrsUsed);

        // Return error if heap read fails.
        if (read_result.ok == false)
        {
            char* error = malloc(256);
            strcpy(error, "ERROR READING COLOR TABLE: ");
            result.data = strcat(error, read_result.data);
            return result;
        }

        // DEBUG: Print that file contains color table.
        #ifdef RUNTIME_DEBUG
        printf("Read Color Table with %u colors.\n",
               bmp->imageHeader.clrsUsed);
        #endif
        // END DEBUG
    }
    else if ((bmp->imageHeader.bitDepth==16 || bmp->imageHeader.bitDepth==32)
             && bmp->imageHeader.compression == 3)
    {
        // Make sure it is set that there is no color table.
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.data = malloc(sizeof(BMPMaskTableHeader_t));
        bmp->bitMaskTable.present = true;

        // Next read the BMP Bitmask Table.
        read_result = heap_read(((BMPMaskTableHeader_t*)bmp->bitMaskTable.data), &bmp_raw,
                                sizeof(BMPMaskTableHeader_t), 1);

        // Return error if heap read fails.
        if (read_result.ok == false)
        {
            char* error = malloc(256);
            strcpy(error, "ERROR READING IMAGE HEADER: ");
            result.data = strcat(error, read_result.data);
            return result;
        }

        // DEBUG: Print out color masks as raw binary data.
        #ifdef RUNTIME_DEBUG
        printf("Read Mask Table | R, G, B.\n");
        print_unsigned_int_binary(
                ((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->red_mask);
        printf("\n");
        print_unsigned_int_binary(
                ((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->green_mask);
        printf("\n");
        print_unsigned_int_binary(
                ((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->blue_mask);
        printf("\n");
        #endif
        // END DEBUG

        // Check that all masks are present and non-zero.
        if ((((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->red_mask == 0
            || ((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->green_mask == 0
            || ((BMPMaskTableHeader_t*)bmp->bitMaskTable.data)->blue_mask == 0)
            && strict_verify)
        {
            result.data = "COLOR CHANNEL HAS NO MASK";
            return result;
        }
    }
    else
    {
        // No mask or Color Table
        bmp->colorTable.present = false;
        bmp->colorTable.data = NULL;

        bmp->bitMaskTable.present = false;
        bmp->bitMaskTable.data = NULL;
    }

    // Check the number of important colors is smaller than the color table.
    if (bmp->imageHeader.clrsImportant > bmp->imageHeader.clrsUsed
        && bmp->colorTable.present)
    {
        result.data = "CLRS IMPORTANT MORE THAN COLORS USED";
        return result;
    }

    // Check if Bitmap is compressed and top down. This is invalid.
    if (bmp->imageHeader.height < 0 && bmp->imageHeader.compression != 0) {
        result.data = "TOP DOWN BITMAPS CANNOT BE COMPRESSED";
        return result;
    }

    //-- Processing File: Read In Pixel Data
    // Verify file pointer position sync.
    if (bmp->fileHeader.pixelOffset - 14 != bmp_raw.position)
    {
        result.data = "PIXEL DATA OFFSET MISMATCH";
        return result;
    }

    //-- Processing File: RLE Decode Branch of Pixel Data Read
    // If the data is RLE8/RLE4 encoded, read and decode the data.
    if (bmp->imageHeader.compression == 1 || bmp->imageHeader.compression == 2)
    {
        // Variables
        bmp->pixelData = malloc(bmp->imageHeader.imageSize);
        result_t rle_result;

        // Read the BMP Pixel Data.
        read_result = heap_read(bmp->pixelData, &bmp_raw,
                                bmp->imageHeader.imageSize, 1);

        // Return error if heap read fails.
        if (read_result.ok == false)
        {
            char* error = malloc(256);
            strcpy(error, "RLE DECODE FAILURE: ");
            result.data = strcat(error, read_result.data);
            return result;
        }

        // Apply the appropriate kind of decode.
        if (bmp->imageHeader.compression == 1)
        {
            // RLE8
            rle_result = rl8_decode(&bmp->pixelData,
                                    bmp->imageHeader.imageSize);
        }
        else if (bmp->imageHeader.compression == 2)
        {
            //RLE 4
            result.ok = false;
            result.data = "RLE-4 IS UNSUPPORTED";
            return result;
        }

        // Check to make sure the RLE result is okay.
        if (rle_result.ok == false)
        {
            char* error = malloc(256);
            strcpy(error, "RLE DECODE FAILURE: ");
            result.data = strcat(error, rle_result.data);
            return result;
        }

        // Set pixel bytes and image size to uncompressed quantity.
        bmp->imageHeader.imageSize = *(u32_t*) rle_result.data;
        pixel_bytes = *(u32_t*) rle_result.data;
    }
    //-- Processing File: Uncompressed data branch of pixel read
    else
    {
        // Determine the final number of bytes of data we should be receiving
        u32_t bits_per_row = bmp->imageHeader.width * bmp->imageHeader.bitDepth;
        if (bmp->imageHeader.bitDepth != 32)
        {
            if (bits_per_row % 32 != 0)
            {
                u32_t padding_needed = 32 - (bits_per_row % 32);
                bits_per_row += padding_needed;
            }
        }
        f64_t bits_count = (f64_t)bits_per_row * (f64_t) abs(bmp->imageHeader.height);
        f64_t bytes = bits_count / 8.0;

        // Check if the number of bytes in the image is larger than a 4 byte int
        // This should never be possible as the BMP file format was not designed
        // to deal with such large image sizes. (Approximately 4GB)
        if (bytes > 4294967295)
        {
            result.data = "IMAGE TOO LARGE";
            return result;
        }
        pixel_bytes = (u32_t) ceil(bytes);

        // Check for uncompressed bitmaps where size does not match expected
        if (bmp->imageHeader.imageSize != 0
            && bmp->imageHeader.imageSize != pixel_bytes
            && bmp->imageHeader.compression == 0)
        {
            result.data = "PIXEL DATA SIZE DOES NOT MATCH EXPECTED";
            return result;
        }

        bmp->pixelData = malloc(pixel_bytes);

        // Read the BMP Pixel Data.
        read_result = heap_read(bmp->pixelData, &bmp_raw,
                                pixel_bytes, 1);

        // Return error if heap read fails.
        if (read_result.ok == false)
        {
            char* error = malloc(256);
            strcpy(error, "PIXEL DATA READ FAILURE: ");
            result.data = strcat(error, read_result.data);
            return result;
        }
    }

    // DEBUG: Print first 4 bytes of pixel data for manual inspection.
    #ifdef RUNTIME_DEBUG
    printf("First 4 bytes: \n");
    for (u32_t i=0; i < 4; i++)
        printf("%#010x ", bmp->pixelData[i]);
    printf("\n");
    #endif
    // END DEBUG

    // Verify that at bit depth 2, we have the appropriate monochrome pallet.
    if (bmp->imageHeader.bitDepth == 1 && bmp->imageHeader.clrsUsed > 2)
    {
        result.data = "COLOR TABLE IS NOT MONOCHROME OR MISSING";
        return result;
    }

    // Verify the pixels are not outside the range of the palette.
    if (bmp->colorTable.present)
    {
        // Ignore 1 BPP as it is impossible for 1 bit to be out of range.

        // Cache this for higher performance.
        u32_t clrs_used_cache = bmp->imageHeader.clrsUsed;

        // Handle 4 BPP
         if (bmp->imageHeader.bitDepth == 4)
         {
            for (u32_t i = pixel_bytes; i > 0; i--)
            {
                u8_t prev_pixel = bmp->pixelData[i - 1];
                u8_t upper = prev_pixel >> 4;
                u8_t lower = prev_pixel & 0x0F;
                if (upper > clrs_used_cache || lower > clrs_used_cache)
                {
                    result.data = "PIXEL COLOR OUTSIDE COLOR TABLE";
                    return result;
                }
            }
         }
         // Handle 8 BPP
         else if (bmp->imageHeader.bitDepth == 8)
         {
            for (u32_t i = pixel_bytes; i > 0; i--)
            {
                if ((u32_t) bmp->pixelData[i - 1] > clrs_used_cache)
                {
                    result.data = "PIXEL COLOR OUTSIDE COLOR TABLE";
                    return result;
                }
            }
        }
    }

    // Make sure to free the buffers
    free(bmp_raw.data);

    result.ok = true;
    result.data = bmp;
    return result;
}
result_t bmp_to_file(FILE* output_file, BMP_t* bmp, option_t key, bool use_compression) {
    result_t result;

    // Write the file header.
    fwrite(&bmp->fileHeader, sizeof(BMPFileHeader_t), 1, output_file);

    // Allocate a heap block to use as temporary buffer for encryption/decryption.
    heapBlock_t heap;
    heap.position = 0;
    heap.length = bmp->fileHeader.size - 12;
    heap.data = malloc(bmp->fileHeader.size - 12);

    // Write the Image Data Header
    heap_write(&heap, &bmp->imageHeader, sizeof(BMPImageHeader_t), 1);

    // Write color table or mask table if present.
    if (bmp->bitMaskTable.present)
    {
        heap_write(&heap, &bmp->bitMaskTable.data, sizeof(BMPImageHeader_t), 1);
    }
    if (bmp->colorTable.present)
    {
        heap_write(&heap, &bmp->colorTable.data, sizeof(BMPImageHeader_t), 1);
    }

    // Deal with compression encodings.
    u8_t** pixelp = &bmp->pixelData;
    u32_t size = bmp->imageHeader.imageSize;
    if (bmp->imageHeader.compression == 1 && bmp->imageHeader.bitDepth != 8 && use_compression)
    {
        result.ok = false;
        result.data = "Attempting to compress non 8-bit file with RLE8.";
        return result;
    }
    else if (bmp->imageHeader.compression == 1 && bmp->imageHeader.bitDepth == 8 && use_compression)
    {
        result_t rle_result = rl8_encode(pixelp, bmp->imageHeader.imageSize, &bmp->imageHeader);

        // Handle Errors
        if (!rle_result.ok)
        {
            result.ok = false;
            result.data = malloc(256);
            strcpy(result.data, "RLE8 Encode Error: ");
            strcat(result.data, rle_result.data);
        }
        size = *(u32_t*) rle_result.data;
    }
    else if (bmp->imageHeader.compression == 2 && bmp->imageHeader.bitDepth != 4 && use_compression)
    {
        result.ok = false;
        result.data = "Attempting to compress non 4-bit file with RLE4.";
        return result;
    }
    else if (bmp->imageHeader.compression == 2 && bmp->imageHeader.bitDepth == 4 && use_compression)
    {
        result.ok = false;
        result.data = "RLE-4 IS UNSUPPORTED";
        return result;
    }
    // No need to do any special processing for compression 3 bitfields or no
    // compression at all.

    heap_write(&heap, *pixelp, size, 1);

    // Finally handle encryption.
    if (key.present)
    {
        result_t xor_result = xor_encrypt(heap.data, heap.length, key.data);

        // Handle Errors
        if (!xor_result.ok)
        {
            result.ok = false;
            result.data = malloc(256);
            strcpy(result.data, "XOR Encryption Error: ");
            strcat(result.data, xor_result.data);
        }
    }

    // And write the heap buffer to the file.
    fwrite(heap.data, heap.position, 1, output_file);

    result.ok = true;
    return result;
}
void bmp_destructor(BMP_t* bmp) {
    if(bmp->colorTable.present)
    {
        free(bmp->colorTable.data);
    }
    if(bmp->bitMaskTable.present)
    {
        free(bmp->bitMaskTable.data);
    }
    free(bmp->pixelData);
    free(bmp);
}
//------------------------------------------------------------------------------
// Private Function Definitions
//------------------------------------------------------------------------------
result_t heap_read(void* dst, heapBlock_t* src, u64_t size, u32_t length) {
    result_t result;
    u32_t read_length = (u32_t) size * length;

    if ((read_length + src->position) > src->length)
    {
        result.ok = false;
        result.data = "NOT ENOUGH BYTES REMAINING IN SRC FOR HEAP READ";
        return result;
    }

    memcpy(dst, src->data + src->position, read_length);

    src->position += read_length;

    result.ok = true;
    result.data = NULL;
    return result;
}
result_t heap_write(heapBlock_t* dst, void* src, u64_t size, u32_t length) {
    result_t result;
    u32_t write_length = (u32_t) size * length;

    if ((dst->length - dst->position) < write_length)
    {
        result.ok = false;
        result.data = "NOT ENOUGH BYTES REMAINING IN HEAP BUFFER TO PREVENT OVERFLOW WHEN WRITING";
        return result;
    }

    memcpy(dst->data + dst->position, src, write_length);

    dst->position += write_length;

    result.ok = true;
    result.data = NULL;
    return result;
}
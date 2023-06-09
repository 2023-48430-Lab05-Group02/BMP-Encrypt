// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Macauley Lim
// -------------------------------------bmp.h-----------------------------------

// Include Guards
#ifndef BMP_H
#define BMP_H

// Standard Library Includes
#include <stdio.h> // File

// Other Includes
#include "datatypes/result.h" // result_t
#include "datatypes/option.h" // option_t
#include "datatypes/short_sizes.h" // u8_t u16_t u32_t i32_t

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------
/*
 * The file header of the BMP file format. This data is contained right at the
 * start of the file and tells us that the file is indeed a BMP file, its size
 * and where the pixel data starts among other attributes.
 * Field Descriptor:
 * type[2] - The characters "BM". Also known as file signature.
 * size - The size of the file as a 4 byte number.
 * reserved1 - Usually reserved, but in our case is used to store the
 * encryption status. 0 Indicates unencrypted, 1 indicates XOR encrypted.
 * reserved2 - Always zero.
 * pixelOffset - 4 byte number represent the offset of the pixel data.
 */
#pragma pack(push, 1)
typedef struct BMPFileHeader {
    u8_t type[2];
    u32_t size;
    u16_t reserved1;
    u16_t reserved2;
    u32_t pixelOffset;
} BMPFileHeader_t;

/*
 * The image data header of the BMP File format. It contains important
 * information used to decode the pixel data from the file.
 * Field Descriptor:
 * size - the size of the header in bytes as 4 byte int. According to the BMP
 * standard this must be at least 40.
 * width - the image width in pixels as a 4 byte int.
 * height - the image height in pixels as a 4 byte int.
 * planes - the number of images planes in the image. This is unused in BMP
 * and is always equal to 1.
 * bitDepth - the number of bits per pixel: 1, 4, 8, 16, 24 or 32.
 * compression - The compression type of the image as a 4 byte int.
 * Valid values include:
 * 0 - Uncompressed,
 * 1 - RLE-8 (8-bit images only),
 * 2 - RLE-4 (4-bit images only),
 * 3 - Bitfields (16 and 32 bit images only)
 * imageSize - The actual size of pixel data in bytes.
 * This can be 0 for images without any compression.
 * xPixelsPerMeter - The DPI of the image horizontally stored as pixels per
 * meter. This may also be 0 if there is no preferred physical dimension.
 * yPixelsPerMeter - The DPI of the image vertically stored as pixels per meter.
 * This may also be 0 if there is no preferred physical dimension.
 * clrsUsed - This value is zero unless the image is less than 8 bits and uses
 * the color table. Then it is the number of colors contained within.
 * clrsImportant - The number of colors that are considered "important" and
 * will be prioritised when rendering with a limited color palette e.g. 16
 * colour LCD. According to the standard, the application reading it must decide
 * how map the other colors to the 16 most important ones. If it is zero, all
 * colors are considered important.
 */
typedef struct BMPImageHeader {
    u32_t size;
    u32_t width;
    i32_t height;
    u16_t planes;
    u16_t bitDepth;
    u32_t compression;
    u32_t imageSize;
    u32_t xPixelsPerMeter;
    u32_t yPixelsPerMeter;
    u32_t clrsUsed;
    u32_t clrsImportant;
} BMPImageHeader_t;

/*
 * Struct that contains 4 u8 values r, g, b, none (alpha) to make accessing
 * the color table data easier.
 */
typedef struct ColorData {
    u8_t red;
    u8_t green;
    u8_t blue;
    u8_t alpha;
} ColorData_t;

/*
 * In an 8 or less bit image, the color table is a palette of colors to render
 * according to the values in the pixel data. We also encrypt the palette as
 * if it were left unencrypted it would allow leaking information about the
 * colors inside the image.
 * Field Descriptors:
 * colorData - Long int representing 4 bytes of data: Red, Green, Blue, None.
 * When reading from the image, we look up the color to render on the terminal
 * or screen from here.
 */
typedef struct BMPColorTableHeader {
    ColorData_t colorData[256];
} BMPColorTableHeader_t;

/*
 * In 16 or 32 bit images, a mask is present in the color table which allows
 * the program to associate arbitrary bits with specific colors.
 * The masks contain a '1' where information should be considered as part of
 * the color and a '0' where it should not be. They are 32 bits in length each,
 * and for a 16 bit image the last two most significant bytes are zero.
 * If any masks overlap, that is an error state.
 */
typedef struct BMPMaskTableHeader {
    u32_t green_mask;
    u32_t red_mask;
    u32_t blue_mask;
} BMPMaskTableHeader_t;
#pragma pack(pop)

/*
 * Putting it all together, this is a complete BMP file.
 */
typedef struct BMP {
    BMPFileHeader_t fileHeader;
    BMPImageHeader_t imageHeader;
    option_t colorTable;
    option_t bitMaskTable;
    u8_t* pixelData;
} BMP_t;

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
/*
 * Reads a file and outputs a BMP if the file is a valid BMP file.
 * Inputs: FILE ptr, XOR key for decryption, whether to strictly verify files.
 * Outputs:
 * result_t with ok = true if the BMP is valid and data: BMP_t ptr.
 * Otherwise, outputs ok = false, with data a char[] message error.
 */
result_t bmp_from_file(FILE* input_file, option_t key, bool strict_verify);
/*
 * Takes a BMP ptr and writes the contents to a file.
 * Inputs: FILE ptr, BMP_t ptr.
 * Outputs:
 * result_t with ok = true if the BMP was successfully written and data nullptr.
 * Otherwise, outputs ok = false, with data a char[] message error.
 */
result_t bmp_to_file(FILE* output_file, BMP_t* bmp, option_t key, bool use_compression);
/*
 * A function that properly frees all loose memory in a BMP. Basically a
 * destructor function. After calling this function, all past acquired pointers
 * are now dangling.
 */
void bmp_destructor(BMP_t* bmp);

#endif //BMP_H

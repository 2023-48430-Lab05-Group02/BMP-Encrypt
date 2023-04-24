// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#ifndef BMP_H
#define BMP_H

#include <stdio.h>

#include "datatypes/result.h"
#include "datatypes/option.h"

/*
 * The file header of the BMP file format. This data is contained right at the
 * start of the file and tells us that the file is indeed a BMP file, its size
 * and where the pixel data starts among other attributes.
 * Field Descriptor:
 * bfType[2] - The characters "BM". Also known as file signature.
 * bfSize - The size of the file as a 4 byte number.
 * bfReserved1 - Usually reserved, but in our case is used to store the
 * encryption status. 0 Indicates unencrypted, 1 indicates XOR encrypted.
 * bfReserved2 - Always zero.
 * bfOffBits - 4 byte number represent the offset of the pixel data.
 */
typedef struct BMPFileHeader {
    char bfType[2];
    unsigned int bfSize;
    unsigned short int bfReserved1;
    unsigned short int bfReserved2;
    unsigned int bfOffBits;
} BMPFileHeader_t;

/*
 * The image data header of the BMP File format. It contains important
 * information used to decode the pixel data from the file.
 * Field Descriptor:
 * biSize - the size of the header in bytes as 4 byte int. According to the BMP
 * standard this must be at least 40.
 * biWidth - the image width in pixels as a 4 byte int.
 * biHeight - the image height in pixels as a 4 byte int.
 * biPlanes - the number of images planes in the image. This is unused in BMP
 * and is always equal to 1.
 * biBitCount - the number of bits per pixel: 1, 4, 8, 16, 24 or 32.
 * biCompression - The compression type of the image as a 4 byte int.
 * Valid values include:
 * 0 - Uncompressed,
 * 1 - RLE-8 (8-bit images only),
 * 2 - RLE-4 (4-bit images only),
 * 3 - Bitfields (16 and 32 bit images only)
 * biSizeImage - The actual size of pixel data in bytes.
 * This can be 0 for images without any compression.
 * biXPelsPerMeter - The DPI of the image horizontally stored as pixels per
 * meter. This may also be 0 if there is no preferred physical dimension.
 * biYPelsPerMeter - The DPI of the image vertically stored as pixels per meter.
 * This may also be 0 if there is no preferred physical dimension.
 * BiClrUsed - This value is zero unless the image is less than 8 bits and uses
 * the color table. Then it is the number of colors contained within.
 * BiClrImportant - The number of colors that are considered "important" and
 * will be prioritised when rendering with a limited color palette e.g. 16
 * colour LCD. According to the standard, the application reading it must decide
 * how map the other colors to the 16 most important ones. If it is zero, all
 * colors are considered important.
 */
typedef struct BMPImageHeader {
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short int biPlanes;
    unsigned short int biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPixelsPerMeter;
    unsigned int biYPixelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BMPImageHeader_t;

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
    unsigned int colorData[256];
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
    unsigned int red_mask;
    unsigned int blue_mask;
    unsigned int green_mask;
} BMPMaskTableHeader_t;

/*
 * Putting it all together, this is a complete BMP file.
 */
typedef struct BMP {
    BMPFileHeader_t fileHeader;
    BMPImageHeader_t imageHeader;
    option_t colorTable;
    option_t bitMaskTable;
    char* pixelData[];
} BMP_t;

/*
 * Reads a file and outputs a BMP if the file is a valid BMP file.
 * Inputs: FILE ptr.
 * Outputs:
 * result_t with ok = true if the BMP is valid and data: BMP_t ptr.
 * Otherwise, outputs ok = false, with data a char[] message error.
 */
result_t bmp_from_file(FILE* input_file);
/*
 * Takes a BMP ptr and writes the contents to a file.
 * Inputs: FILE ptr, BMP_t ptr.
 * Outputs:
 * result_t with ok = true if the BMP was successfully written and data nullptr.
 * Otherwise, outputs ok = false, with data a char[] message error.
 */
result_t bmp_to_file(FILE* output_file, BMP_t* bmp);

#endif //BMP_H
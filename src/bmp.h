// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#ifndef BMP_H
#define BMP_H

#include <stdio.h>

#include "datatypes/result.h"

typedef struct BMPFileHeader {
    char bfType[2];
    unsigned long int bfSize;
    unsigned int bfReserved1;
    unsigned int bfReserved2;
    unsigned long int bfOffBits;
} BMPFileHeader_t;

typedef struct BMPImageHeader {
    unsigned long int biSize;
    unsigned long int biWidth;
    unsigned long int biHeight;
    unsigned int biPlanes;
    unsigned int biBitCount;
    unsigned long int biCompression;
    unsigned long int biSizeImage;
    unsigned long int biXPixelsPerMeter;
    unsigned long int biYPixelsPerMeter;
    unsigned long int biClrUsed;
    unsigned long int biClrImportant;
} BMPImageHeader_t;

typedef struct BMP {
    BMPFileHeader_t file_header;
    BMPImageHeader_t image_header;
    char* pixel_data[];
} BMP_t;

result bmp_from_file(FILE* input_file, BMP_t* bmp);
result bmp_to_file(FILE* output_file, BMP_t* bmp);

#endif //BMP_H

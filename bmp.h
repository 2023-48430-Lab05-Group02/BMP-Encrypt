//
// Created by Macauley Lim on 20/04/23.
//

#ifndef BMP_H
#define BMP_H

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

#endif //BMP_H

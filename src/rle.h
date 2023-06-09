// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Benjamin Hudson
// ----------------------------------rle.h--------------------------------------

// Include Guard
#ifndef RLE_H
#define RLE_H

// Other Includes
#include "datatypes/result.h" // result_t
#include "datatypes/short_sizes.h" // u8_t

#include "bmp.h" // BMPImageHeader_t

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
/*
 * These functions perform rl8 decode or encode tasks.
 * Inputs:
 * - char* data[], the raw data to encode or decode.
 * - int length - the length of the raw data.
 * Outputs:
 * result_t with ok = true, data = u32 new length.
 * Or result_t with ok = false and data char[] error message.
 */
result_t rl8_encode(u8_t** input, BMPImageHeader_t* image_header);
result_t rl8_decode(u8_t** input, BMPImageHeader_t* image_header);

#endif //RLE_H

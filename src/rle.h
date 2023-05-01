// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File: Benjamin Hudson

#ifndef RLE_H
#define RLE_H

#include "datatypes/result.h"
#include "datatypes/short_sizes.h"

/*
 * These functions perform rl8 decode or encode tasks.
 * Inputs:
 * - char* data[], the raw data to encode or decode.
 * - int length - the length of the raw data.
 * Outputs:
 * result_t with ok = true, data = u32 new length.
 * Or result_t with ok = false and data char[] error message.
 */
result_t rl8_encode(u8** data, u32 length);
result_t rl8_decode(u8** data, u32 length);

/*
 * These functions perform rl4 decode or encode tasks.
 * Inputs:
 * - char* data[], the raw data to encode or decode.
 * - int length - the length of the raw data.
 * Outputs:
 * result_t with ok = true, data = char* data[] heap pointer with the new data.
 * Or result_t with ok = false and data char[] error message.
 */
result_t rl4_encode(u8** data, u32 length);
result_t rl4_decode(u8** data, u32 length);

#endif //RLE_H

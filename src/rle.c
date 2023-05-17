// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "rle.h"

result_t rl8_encode(u8_t** data, u32_t length, BMPImageHeader_t* image_header) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    int hight_count, width_count, count;
    unsigned char *output, current_byte;
    output = (unsigned char*)malloc(sizeof(data) * 2);
    /* add test to see if output true*/

    u32* location_counter = malloc((sizeof(u32)));

    location_counter = 0;

    for (hight_count = 0; hight_count < (int)image_header->height; hight_count++) {
        width_count = 0;

        while (width_count < (int)image_header->width) {
            *current_byte = data[hight_count * (int)image_header->width + width_count];
            count = 1;

            while (count < 255 && width_count + count < (int)image_header->width && data[hight_count * (int)image_header
                  ->width + width_count + count] == data[hight_count * (int)image_header->width + width_count]) {
                count++;
            }
            /* it would be better to compress unique pixels in absolute mode, as just using encoded mode can increase
             * the total file size if there is alot of unique pixels, might add later */

            output[*location_counter++] = (unsigned char) count;
            output[*location_counter++] = current_byte;

            width_count += count;
        }

        output[*location_counter++] = 0;
        output[*location_counter++] = 0;
    }
    output[*location_counter++] = 0;
    output[*location_counter++] = 1;


    /* create test to make sure result.test is true */
    result.data = output;
    
    return result;
}
result_t rl8_decode(u8_t** data, u32_t length) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

result_t rl4_encode(u8_t** data, u32_t length, BMPImageHeader_t* image_header) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    int hight_count, width_count, count, shift;
    unsigned char *output, current_byte;
    output = (unsigned char*)malloc(sizeof(data) * 2);
    /* add test to see if output true*/

    u32* location_counter = malloc((sizeof(u32)));

    location_counter = 0;

    for (hight_count = 0; hight_count < (int)image_header->height; hight_count++) {
        width_count = 0;

        while (width_count < (int)image_header->width) {
            *current_byte = data[hight_count * (int)image_header->width + width_count];
            count = 2;
            shift = 1;

            while (count < 255 && width_count + shift < (int)image_header->width && data[hight_count * (int)image_header
                    ->width + width_count + shift] == data[hight_count * (int)image_header->width + width_count]) {
                count++;
                count++;
                shift++;
            }

            /* it would be better to compress unique pixels in absolute mode, as just using encoded mode can increase
             * the total file size if there is alot of unique pixels, might add later */

            output[*location_counter++] = (unsigned char) count;
            output[*location_counter++] = current_byte;

            width_count += shift;
        }

        output[*location_counter++] = 0;
        output[*location_counter++] = 0;
    }
    output[*location_counter++] = 0;
    output[*location_counter++] = 1;


    /* create test to make sure result.test is true */
    result.data = output;

    return result;
}
result_t rl4_decode(u8_t** data, u32_t length) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

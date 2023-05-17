// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "rle.h"

result_t rl8_encode(u8_t** data, u32_t length, BMPImageHeader_t* image_header) {
    result_t result;

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
             * the total file size if there is a lot of unique pixels, might add later */

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
    result.ok = true;

    free(data);

    return result;
}
result_t rl8_decode(u8_t** data, u32_t length, BMPImageHeader_t* image_header) {
    result_t result;
    unsigned char *output, current_byte;
    output = (unsigned char*)malloc(1);
    int step = 1;
    u32* location_counter = malloc((sizeof(u32)));
    location_counter = 0;

    while (data[step] != 0 && data[step + 1] != 1){
        *current_byte = data[step];

        if (current_byte == 0) {

            if (data[step + 1] < 3) {
                step++;

                if (data[step] == 2){

                    step++;
                    int section_size = 0;
                    section_size = (int)data[step]+((int)image_header->width * (int)data[step + 1]);
                    output = (unsigned char*)realloc(output, location_counter + section_size + 1);
                    step++

                    while(section_size > 0){
                        output[location_counter++] = 0;
                        section_size--;
                    }

                }
            }

            else{
            int subcount = 1;
            while (data[step + subcount] != 0){
                output = (unsigned char*)realloc(output, location_counter + 1)
                output[location_counter++] = data[step + subcount];
                subcount++;
            }
            step = step + subcount;
        }}
        else{
            int subcount = current_byte;
            while (subcount > 0){
                output = (unsigned char*)realloc(output, location_counter + 1)
                output[location_counter++] = data[step + 1]
            }
            step ++;
        }
        step ++;

    }


    result.data = output;
    result.ok = true;

    return result;
}

result_t rl4_encode(u8_t** data, u32_t length, BMPImageHeader_t* image_header) {
    result_t result;

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
             * the total file size if there is a lot of unique pixels, might add later*/

            output[*location_counter++] = (unsigned char) count;
            output[*location_counter++] = current_byte;

            width_count += shift;
        }

        output[*location_counter++] = 0;
        output[*location_counter++] = 0;
    }
    output[*location_counter++] = 0;
    output[*location_counter++] = 1;


    free(data);

    /* create test to make sure result.test is true */
    result.data = output;
    result.ok = true;

    return result;
}
result_t rl4_decode(u8_t** data, u32_t length, BMPImageHeader_t* image_header) {
    result_t result;

    result.ok = false;
    result.data = "CODE INCOMPLETE";

    return result;
}

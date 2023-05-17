// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "rle.h"
#include <stdlib.h>

void* safe_realloc(void* pointer, int size);

result_t rl8_encode(u8_t** data, BMPImageHeader_t* image_header) {
    result_t result;


    int hight_count, width_count, count, length;
    unsigned char *output, current_byte;
    output = malloc(sizeof(data) * 2);
    /* add test to see if output true*/
    length = 0;
    int location_counter = 0;

    for (hight_count = 0; hight_count < (int)image_header->height; hight_count++) {
        width_count = 0;

        while (width_count < (int)image_header->width) {
            current_byte = *data[hight_count * (int)image_header->width + width_count];
            count = 1;

            while (count < 255 && width_count + count < (int)image_header->width && data[hight_count * (int)image_header
                  ->width + width_count + count] == data[hight_count * (int)image_header->width + width_count]) {
                count++;
            }
            /* it would be better to compress unique pixels in absolute mode, as just using encoded mode can increase
             * the total file size if there is a lot of unique pixels, might add later */

            output[location_counter++] = (unsigned char) count;
            output[location_counter++] = current_byte;

            width_count += count;
        }

        output[location_counter++] = 0;
        output[location_counter++] = 0;
    }
    output[location_counter++] = 0;
    output[location_counter++] = 1;
    image_header->imageSize = (u32_t)location_counter;

    /* create test to make sure result.test is true */
    result.data = output;
    result.ok = true;

    free(data);

    return result;
}
result_t rl8_decode(u8_t** data, BMPImageHeader_t* image_header) {
    result_t result;
    unsigned char *output, current_byte;
    output = malloc(1);
    int step = 1;
    int location_counter = 0;

    while (data[step] != 0 && *data[step + 1] != 1){
        current_byte = *data[step];

        if (current_byte == 0) {

            if (*data[step + 1] < 3) {
                step++;

                if (*data[step] == 2){

                    step++;
                    int section_size = 0;
                    section_size = *data[step]+((int)image_header->width * *data[step + 1]);
                    output = safe_realloc(output, location_counter + section_size + 1);
                    step++;

                    while(section_size > 0){
                        output[location_counter++] = 0;
                        section_size--;
                    }

                }
            }

            else{
                step++;
                int subcount = *data[step];
                output = safe_realloc(output, location_counter + subcount);
                step++;
                while (subcount > 0){
                    output[location_counter++] = *data[step];
                    step++;
                    subcount--;
                }
                if(subcount % 2 != 0){
                    step++;
                }
            }
        }
        else{
            int subcount = current_byte;
            while (subcount > 0){
                output = safe_realloc(output, location_counter + 1);
                output[location_counter++] = *data[step + 1];
            }
            step ++;
        }
        step ++;

    }

    image_header->imageSize = (u32_t)location_counter;
    result.data = output;
    result.ok = true;

    return result;
}

void* safe_realloc(void* pointer, int size){
    void* output;
    output = realloc(pointer, size);

    if (output == NULL){
        output = malloc(size);
        memcpy(output, pointer, sizeof(*pointer));
    }
    return output;
}

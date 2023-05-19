// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "rle.h"
#include <stdlib.h>
#include <string.h>
#include "./util/realloc.h"

result_t rl8_encode(u8_t** data, BMPImageHeader_t* image_header) {
    result_t result;

    int hight_count, width_count, count;
    unsigned char *output, current_byte;
    output = malloc(sizeof(*data) * 2);
    /* add test to see if output true*/
    int location_counter = 0;

    for (hight_count = 0; hight_count < (int)image_header->height;
         hight_count++) {

        width_count = 0;

        while (width_count < (int)image_header->width) {
            current_byte = *data[hight_count * (int)image_header->width +
                                 width_count];
            count = 1;

            while (count < 255 && width_count + count < (int)image_header->width
                   && *data[hight_count * (int)image_header->width + width_count
                   + count] == *data[hight_count * (int)image_header->width +
                   width_count]) {

                count++;
            }

            if (count == 1){
                int subcount = 0;
                while (*data[hight_count * (int)image_header->width +
                       width_count + count + subcount] != *data[hight_count *
                       (int)image_header->width + width_count + count + subcount
                       + 1] && width_count + count + subcount <
                       (int)image_header->width){

                    subcount++;
                }

                subcount++; /*accounts for initial byte*/

                if (*data[hight_count * (int)image_header->width + width_count +
                    count + subcount] == *data[hight_count *
                    (int)image_header->width + width_count + count + subcount +
                    1]) {

                    subcount--; /*accounts for if last byte is start of encoded
 *                                mode */
                }

                output[location_counter++] = (unsigned char)0; /*start absolute
 *                                                               mode */
                output[location_counter++] = (unsigned char)subcount;
                /*states number of bytes to follow */

                int subsubcount = 0;
                while (subsubcount <= subcount){
                    output[location_counter++] = *data[hight_count *
                    (int)image_header->width + width_count + subsubcount];

                    subsubcount++;
                }

                subcount--; /* doesn't add initial byte to width_count as it
 *                             will be added later */
                width_count += subcount;

            }
            else {
                output[location_counter++] = (unsigned char)count;
                output[location_counter++] = current_byte;
            }

            width_count += count;
        }

        output[location_counter++] = (unsigned char)0;
        output[location_counter++] = (unsigned char)0;
    }
    output[location_counter++] = (unsigned char)0;
    output[location_counter++] = (unsigned char)1;
    image_header->imageSize = (u32_t)location_counter;

    result.ok = true;
    safe_realloc(output, (u32_t)location_counter);
    if (output == NULL){
        result.ok =false;
    }
    result.data = output;

    free(data);

    return result;
}
result_t rl8_decode(u8_t** data, BMPImageHeader_t* image_header) {
    result_t result;
    unsigned char *output, current_byte;
    output = malloc(1);
    int step = 1;
    int location_counter = 0;

    while (*data[step] != 0 && *data[step + 1] != 1){
        current_byte = *data[step];

        if (current_byte == 0) {

            if (*data[step + 1] < 3) {
                step++;

                if (*data[step] == 2){

                    step++;
                    int section_size;
                    section_size = *data[step]+((int)image_header->width *
                                   *data[step + 1]);
                    output = safe_realloc(output, (u32_t)
                    (location_counter + section_size + 1));
                    step++;

                    while(section_size > 0){
                        output[location_counter++] = (unsigned char)0;
                        section_size--;
                    }

                }
            }

            else{
                step++;
                int subcount = *data[step];
                output = safe_realloc(output, (u32_t)
                (location_counter + subcount));

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
                output = safe_realloc(output, (u32_t)
                (location_counter + 1));

                output[location_counter++] = *data[step + 1];
                subcount--;
            }
            step ++;
        }
        step ++;

    }

    image_header->imageSize = (u32_t)location_counter;
    result.ok = true;
    if (output == NULL){
        result.ok =false;
    }
    result.data = output;

    return result;
}
// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Benjamin Hudson
// ----------------------------------rle.c--------------------------------------

// Standard Library Includes
#include <stdlib.h> // malloc free

// Public API Includes
#include "rle.h"

// Other Includes
#include "datatypes/result.h" // result_t

#include "bmp.h" // BMPImageHeader_t
#include "util/realloc.h" // safe_realloc

result_t rl8_encode(u8_t** input, BMPImageHeader_t* image_header) {
    result_t result;
    u8_t* data = *input; //solves mystery seg fault, I HATE THAT THIS WORKS
    i32_t height_count, width_count, count; //initialize variables
    u8_t current_byte;
    u8_t *output = malloc((image_header->imageSize * 2) + (u32_t)(image_header->height * 2) + 2); //max decompression can be 2 * the size

    /* add test to see if output true*/
    int location_counter = 0;


    for (height_count = 0; height_count < (int)image_header->height; height_count++) { //main for loop

        width_count = 0;

        while (width_count < (int)image_header->width) { //loop for every line of bmp
            current_byte = data[(height_count * (int)image_header->width) + width_count];
            count = 1;

            while (count < 255
            && width_count + count < (int)image_header->width
            && data[(height_count * (int)image_header->width) + width_count + count]
            == current_byte) {

                count++;
            }

            if (count == 1){ //if no same pixels must be in absolute mode
                int subcount = 0; //finds amount of unique pixels in row
                while (data[height_count * (int)image_header->width + width_count + count + subcount]
                != data[height_count * (int)image_header->width + width_count + count + subcount + 1]
                && width_count + count + subcount < (int)image_header->width){

                    subcount++;
                }

                subcount++; /*accounts for initial byte*/

                if (subcount < 3){ //if < 3 will be recognised as an escape sequence or delta
                    output[location_counter++] = (unsigned char)count;
                    output[location_counter++] = current_byte;
                }

                else { // write data in absolute mode
                    if (data[height_count * (int) image_header->width + width_count + count + subcount] == data[height_count * (int) image_header->width + width_count + count + subcount + 1]) {

                        subcount--; /*accounts for if last byte is start of encoded mode */
                    }

                    output[location_counter++] = (unsigned char) 0; /*start absolute mode */
                    output[location_counter++] = (unsigned char) subcount; /*states number of bytes to follow */

                    int subsubcount = 0;
                    while (subsubcount <= subcount) { //adds colour data in absolute mode

                        output[location_counter++] = data[height_count * (int) image_header->width + width_count + subsubcount];
                        subsubcount++;
                    }
                    if(subcount % 2 != 0) { //fills out to 16bit word length
                        output[location_counter++] = (unsigned char) 0;
                    }


                    subcount--; /* doesn't add initial byte to width_count as it will be added later */
                    width_count += subcount;
                }

            }
            else { //if there is a row of like pixels, puts the amount then the colour for absolute mode
                output[location_counter++] = (unsigned char)count;
                output[location_counter++] = current_byte;
            }

            width_count += count;
        }
        if (height_count < image_header->height) {
            output[location_counter++] = (unsigned char) 0; //end of line character
            output[location_counter++] = (unsigned char) 0;
        }
    }
    output[location_counter++] = (unsigned char)0; //end of file character
    output[location_counter++] = (unsigned char)1;
    image_header->imageSize = (u32_t)location_counter; //set new image length

    result.ok = true;
    output = safe_realloc(output, (u32_t)location_counter); //put new data in allocation right size for it
    if (output == NULL){ //test if realloc worked
        result.data = "REALLOC FAIL";
        result.ok = false;
    }

    *input = output;

    free(data); //free the allocation with input data

    result.ok = true;
    return result;
}
result_t rl8_decode(u8_t** input, BMPImageHeader_t* image_header) {
    result_t result;
    u8_t* data = *input;
    unsigned char *output, current_byte; //initialise variables
    output = malloc(1);
    int step = 0;
    int location_counter = 0;

    while (!(data[step] == 0 && data[step + 1] == 1)){ //main loop
        current_byte = data[step];

        if (current_byte == 0) { //tests for absolute mode

            if (data[step + 1] < 3) { //test for escape character or delta
                step++;

                if (data[step] == 2){ //test for delta

                    step++;
                    int section_size;
                    section_size = data[step]+((int)image_header->width * data[step + 1]); //calculate amount of pixels to be skipped
                    output = safe_realloc(output, (u32_t)(location_counter + section_size + 1)); //allocates new memory
                    step++;

                    while(section_size > 0){
                        output[location_counter++] = (unsigned char)0; //sets skipped pixels to 0
                        section_size--;
                    }

                }
            }

            else{ //if absolute mode
                step++;
                int subcount = data[step]; //gets amount of unique pixels
                output = safe_realloc(output, (u32_t)(location_counter + subcount)); //allocates new memory

                step++;
                while (subcount > 0){ //takes data from absolute mode
                    output[location_counter++] = data[step];
                    step++;
                    subcount--;
                }
                if(subcount % 2 != 0){ //accounts for filling out work length
                    step++;
                }
            }
        }
        else{ //if in encoded mode
            int subcount = current_byte;
            while (subcount > 0){ //takes data out of encoded mode
                output = safe_realloc(output, (u32_t)
                (location_counter + 1));

                output[location_counter++] = data[step + 1];
                subcount--;
            }
            step ++;
        }
        step ++;

    }

    image_header->imageSize = (u32_t)location_counter; //set new image length
    result.ok = true;
    if (output == NULL){ //test if realloc worked
        result.data = "REALLOC FAIL";
        result.ok = false;
    }

    *input = output;

    free(data); //free input allocation

    result.ok = true;
    return result;
}
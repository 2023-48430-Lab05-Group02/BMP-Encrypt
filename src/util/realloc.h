//
// Created by Macauley Lim on 17/05/23.
//

#ifndef REALLOC_H
#define REALLOC_H

#include <stdlib.h>
#include <string.h>

#include "datatypes/short_sizes.h"

void* safe_realloc(void* pointer, u32_t size){
    void* output;
    output = realloc(pointer, size);

    if (output == NULL){
        output = malloc(size);
        memcpy(output, pointer, sizeof(*pointer));
    }
    return output;
}

#endif //REALLOC_H

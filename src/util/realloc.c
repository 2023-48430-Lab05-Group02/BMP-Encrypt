//
// Created by Macauley Lim on 17/05/23.
//

#include <stdlib.h>
#include <string.h>

#include "realloc.h"

void* safe_realloc(void* pointer, u32_t size) {
    void* output;
    output = realloc(pointer, size);

    if (output == NULL){
        output = malloc(size);
        memcpy(output, pointer, sizeof(*pointer));
    }
    return output;
}

//
// Created by Macauley Lim on 21/04/2023.
//

#ifndef RESULT_H
#define RESULT_H

#include "bool.h"

/*
 * A way of representing whether a function has returned a valid result or not.
 * If ok is false, data must contain a char[] with an error message.
 * Else, data is the return value of the function.
 */
typedef struct result_struct {
    bool ok;
    void* data;
} result_t;

#endif //RESULT_H

//
// Created by Macauley Lim on 24/04/23.
//

#ifndef OPTION_H
#define OPTION_H

#include "bool.h"

/*
 * A way of representing an optional. Either the value is present or not.
 * This helps to ensure the safety of the code by ensuring allowing code to
 * ensure a value is present or handle if it isn't.
 * Data is only present if present is true. Otherwise, data is a nullptr.
 */
typedef struct option_struct {
    bool present;
    void* data;
} option_t;

option_t None = {false, NULL};

#endif //OPTION_H

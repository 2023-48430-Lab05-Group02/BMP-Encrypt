// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#ifndef RLE_H
#define RLE_H

#include "datatypes/result.h"

result rl8_encode(char* data[], int length);
result rl8_decode(char* data[], int length);

result rl4_encode(char* data[], int length);
result rl4_decode(char* data[], int length);

#endif //RLE_H

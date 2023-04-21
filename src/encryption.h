// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "datatypes/result.h"

// Static Defines
#define XOR_KEY_SIZE 256

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

result xor_encrypt(char* data[], int length, char key[]);
result xor_decrypt(char* data[], int length, char key[]);

#endif //ENCRYPTION_H

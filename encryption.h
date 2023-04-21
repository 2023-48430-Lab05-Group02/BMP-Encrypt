//
// Created by Macauley Lim on 20/04/23.
//
#include "datatypes/result.h"

// Static Defines
#define XOR_KEY_SIZE 256

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

result xor_encrypt(char* data[], int length, char key[]);
result xor_decrypt(char* data[], int length, char key[]);

#endif //ENCRYPTION_H

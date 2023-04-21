//
// Created by Macauley Lim on 20/04/23.
//
#include "datatypes/bool.h"

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

bool xor_encrypt(char* data[], int length, char key[], int key_size);
bool xor_decrypt(char* data[], int length, char key[], int key_size);

#endif //ENCRYPTION_H

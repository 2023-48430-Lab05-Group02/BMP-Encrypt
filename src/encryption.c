// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Osmaan Ahmad
// ---------------------------encryption.c--------------------------------------

// Standard Library Includes
#include <stdio.h>
#include <string.h>

// Public API includes
#include "encryption.h"

//------------------------------------------------------------------------------
// Public Function Definitions
//------------------------------------------------------------------------------
u32_t fnv1a_hash(const char* str, u64_t length) {
    u32_t hash = FNV_OFFSET_BIAS;

    for (u64_t i = 0; i < length; i++)
    {
        hash ^= (unsigned char) str[i];
        hash *= FNV_PRIME;
    }

    return hash;
}
void xor_encrypt(char* data, u32_t length, const u32_t* key) {
    // Variables
    i8_t keys[4];
    u32_t rem;
    memcpy(keys, key, 4);

    for (u32_t i = 0; i < length; i++)
    {
        rem = length % 4;
        // Apply XOR operation using the generated key
        data[i] = data[i] ^ keys[rem];
    }
}
void xor_decrypt(char* data, u32_t length, u32_t* key) {
    xor_encrypt(data, length, key);
}

// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Osmaan Ahmad
// ---------------------------encryption.c--------------------------------------

// Standard Library Includes
#include <stdio.h>

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
    u32_t* block_ptr = (u32_t*) data;
    u32_t block_len = length / 4;

    for (u32_t i = 0; i < block_len; i++)
    {
        // Apply XOR operation using the generated key
        block_ptr[i] = block_ptr[i] ^ *key;
    }
}
void xor_decrypt(char* data, u32_t length, u32_t* key) {
    xor_encrypt(data, length, key);
}

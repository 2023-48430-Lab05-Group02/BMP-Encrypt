// ---------------------------------BMP-Encrypt---------------------------------
// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Copyright © 2023 Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad.
// Primary File Contributor: Osmaan Ahmad
// ---------------------------encryption.h--------------------------------------

//Include Guards
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

// Other Includes
#include "datatypes/result.h"
#include "datatypes/short_sizes.h"

// Static Defines
#define FNV_PRIME 16777619u
#define FNV_OFFSET_BIAS 2166136261u

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
/*
 * This function preforms the Fowler-Noll-Vo hash function type 1A. It is a
 * simple to implement way to produce a fixed length key from a password.
 * Inputs:
 * const char* str[] which is the password.
 * u64_t length which is the password length.
 * Outputs:
 * u32_t which is the hashed value.
 */
u32_t fnv1a_hash(const char* str, u64_t length);
/*
 * Encrypts data with a xor key.
 * Inputs:
 * - char* data - The data that is to be encrypted.
 * - u32_t length - The length of the data that is to be encrypted.
 * - u32_t* key - A pointer to the key that should be used for encryption.
 */
void xor_encrypt(char* data, u32_t length, const u32_t* key);
/*
 * Decrypts data with a xor key.
 * Inputs:
 * - char* data - The data that is to be decrypted.
 * - int length - The length of the data that is to be decrypted.
 * - u32_t* key - A pointer to the key that should be used for decryption.
 */
void xor_decrypt(char* data, u32_t length, u32_t* key);

#endif //ENCRYPTION_H

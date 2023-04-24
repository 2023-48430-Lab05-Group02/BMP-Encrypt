// 48430 Introduction to C Programming
// Lab: 5, Group: 2
// Created by Benjamin Hudson, Joseph Rico, Macauley Lim, Osmaan Ahmad
// Person Responsible For File:

#include "datatypes/result.h"

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

// Static Defines
#define XOR_KEY_SIZE 32
#define FNV_PRIME 16777619u
#define FNV_OFFSET_BIAS 2166136261u

/*
 * This function preforms the Fowler-Noll-Vo hash function type 1A. It is a
 * simple to implement way to produce a fixed length key from a password.
 * Inputs:
 * const char* str[] which is the password.
 * Outputs:
 * unsigned char which is the hashed value.
 */
unsigned char fnv1a_hash(const char* str);
/*
 * Encrypts data with a xor key.
 * Inputs:
 * - char data[] - The data that is to be encrypted.
 * - int length - The length of the data that is to be encrypted.
 * - char* key[] - A pointer to they key that should be used for encryption.
 * The length of char* key[] must be XOR_KEY_SIZE.
 * Outputs:
 * result_t with ok = true, and data = char* data[] with the returned data.
 * Otherwise, result_T with ok = false, and a char[] error message.
 */
result_t xor_encrypt(char data[], int length, char* key[]);
/*
 * Decrypts data with a xor key.
 * Inputs:
 * - char data[] - The data that is to be decrypted.
 * - int length - The length of the data that is to be decrypted.
 * - char* key[] - A pointer to they key that should be used for decryption.
 * The length of char* key[] must be XOR_KEY_SIZE.
 * Outputs:
 * result_t with ok = true, and data = char* data[] with the returned data.
 * Otherwise, result_T with ok = false, and a char[] error message.
 */
result_t xor_decrypt(char data[], int length, char* key[]);

#endif //ENCRYPTION_H
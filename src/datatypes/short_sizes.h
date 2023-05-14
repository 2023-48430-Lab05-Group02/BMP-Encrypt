//
// Created by Macauley Lim on 02/05/2023.
//

#ifndef SHORT_SIZES_H
#define SHORT_SIZES_H

/*
 * -- Explanation Of Type Names --
 * These type names follow rust terminology.
 * e.g. 'u' means unsigned, 'i' means signed.
 * This should be followed immediately by the number of bits the value takes up
 * in memory.
 *
 * Care should be taken to remember that the max value of a signed type is half
 * that of an unsigned type of the same bit count.
 */

/*
 * Important notes on the return types of various functions:
 * sizeof() - u64
 */

// Unsigned integers
typedef unsigned char u8_t;
typedef unsigned short int u16_t;
#ifdef _WIN64
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
#elif __linux__
typedef unsigned int u32_t;
typedef unsigned long u64_t;
#endif

// Unsigned integers min values (inclusive):
#define MIN_u8 0
#define MIN_u16 0
#define MIN_u32 0
#define MIN_u64 0

// Unsigned integers max values (inclusive):
#define MAX_u8 255
#define MAX_u16 65535
#define MAX_u32 2147483647
#define MAX_u64 18446744073709551615

// Signed integers
typedef char i8_t;
typedef short int i16_t;
#ifdef _WIN64
typedef int i32_t;
typedef long long i64_t;
#elif __linux__
typedef int i32_t;
typedef long i64_t;
#endif

// Unsigned integers min values (inclusive):
#define MIN_i8 (-128)
#define MIN_i16 (-32768)
#define MIN_i32 (-2147483648)
#define MIN_i64 (-9223372036854775808)

// Unsigned integers max values (inclusive):
#define MAX_i8 127
#define MAX_i16 32767
#define MAX_i32 2147483647
#define MAX_i64 9223372036854775807

// Floating point
typedef float f32_t;
typedef double f64_t;

// Floating point min values (inclusive):
#define MIN_f32 1.2E-38
#define MIN_f64 2.3E-308

// Floating point max values (inclusive):
#define MAX_f32 3.4E+38
#define MAX_f64 1.7E+308

#endif //SHORT_SIZES_H

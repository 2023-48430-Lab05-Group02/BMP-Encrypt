//
// Created by Macauley Lim on 20/04/2023.
//

#ifndef BOOL_H
#define BOOL_H

/*
 * Is either true (1) or false (0). Just a nicer way to interact with these kind
 * of on or off values without being able to use the stdbool.h library.
 */
typedef enum bool_enum {
    false = 0,
    true = 1
} bool;

#endif //BOOL_H

//
// Created by Macauley Lim on 20/04/23.
//

#ifndef RLE_H
#define RLE_H

#include "datatypes/result.h"

result rl8_encode(char* data[], int length);
result rl8_decode(char* data[], int length);

result rl4_encode(char* data[], int length);
result rl4_decode(char* data[], int length);

#endif //RLE_H

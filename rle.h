//
// Created by Macauley Lim on 20/04/23.
//

#ifndef RLE_H
#define RLE_H

int rl8_encode(char* data[], int length);
int rl8_decode(char* data[], int length);

int rl4_encode(char* data[], int length);
int rl4_decode(char* data[], int length);

#endif //RLE_H

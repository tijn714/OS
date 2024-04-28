#ifndef BASE64_H
#define BASE64_H

#include "types.h"

int base64_index(char c);
void base64_encode(uint8_t in[], int len, char out[]);
void base64_decode(char in[], int len, uint8_t out[]);

#endif // BASE64_H
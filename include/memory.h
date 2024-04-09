#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void memset(void *dest, uint8 val, uint32 len);
void memcpy(void *dest, const void *src, uint32 len);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
int strlen(const char *str);
char *itoa(int value, char *str, int base);

#endif // MEMORY_H
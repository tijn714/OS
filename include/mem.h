#ifndef MEM_H
#define MEM_H

#include "types.h"

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
void memset(uint8_t *dest, uint8_t val, uint32_t len);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, uint32_t len);
char *strstr(const char *haystack, const char *needle);
char *itoa(int num, char *str, int base);
void upper(char *str);
void lower(char *str);
size_t strlen(const char *str);


bool isEqual(uint8_t *a, uint8_t *b, int len);

#endif // MEM_H
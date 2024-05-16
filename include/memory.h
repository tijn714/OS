#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

// strlen
size_t strlen(const char* str);

// itoa
char* itoa(int value, char* str, int base);

//memset
void* memset(void* ptr, int value, size_t num);

//memcpy
void* memcpy(void* dest, const void* src, size_t num);

// strstr
char* strstr(const char* haystack, const char* needle);

// is functions
bool isEqual(const uint8_t* a, const uint8_t* b, size_t size);
bool islower(char c);
bool isupper(char c);
bool isint(char c);
bool isdigit(char c);

#endif /* MEMORY_H */
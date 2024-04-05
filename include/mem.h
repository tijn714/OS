#ifndef MEM_H
#define MEM_H

#include "types.h"

size_t strlen(const char* str);

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);
void itoa(int n, char* str, int base);


size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void* memset(void* ptr, int value, size_t num)
{
    byte* p = (byte*)ptr;
    for (size_t i = 0; i < num; i++)
        p[i] = (byte)value;
    return ptr;
}

void* memcpy(void* dest, const void* src, size_t num)
{
    byte* d = (byte*)dest;
    const byte* s = (const byte*)src;
    for (size_t i = 0; i < num; i++)
        d[i] = s[i];
    return dest;
}

void itoa(int n, char* str, int base)
{
    int i = 0;
    bool is_negative = false;
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (n < 0 && base == 10) {
        is_negative = true;
        n = -n;
    }
    while (n != 0) {
        int rem = n % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n = n / base;
    }
    if (is_negative)
        str[i++] = '-';
    str[i] = '\0';
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

#endif // MEM_H
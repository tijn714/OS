#include "types.h"
#include "memory.h"


void memset(void *dest, uint8 val, uint32 len) {
    uint8 *temp = (uint8 *)dest;
    for( ; len != 0; len--) *temp++ = val;
}

void memcpy(void *dest, const void *src, uint32 len) {
    const uint8 *sp = (const uint8 *)src;
    uint8 *dp = (uint8 *)dest;
    for( ; len != 0; len--) *dp++ = *sp++;
}

int strcmp(const char *str1, const char *str2) {
    while(*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

char *strcpy(char *dest, const char *src) {
    char *temp = dest;
    while(*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return temp;
}

int strlen(const char *str) {
    int len = 0;
    while(str[len]) len++;
    return len;
}

char *itoa(int value, char *str, int base) {
    char *rc;
    char *ptr;
    char *low;
    if(base < 2 || base > 36) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    if(value < 0 && base == 10) *ptr++ = '-';
    low = ptr;
    do {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while(value);
    *ptr-- = '\0';
    while(low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}
#ifndef __SYS_H__
#define __SYS_H__

#include "types.h"

// extern uint8_t* memcpy(uint8_t* dst, const uint8_t* src, int32_t cnt);
// extern uint8_t* memset(uint8_t* dst, uint8_t val, int32_t cnt);
// extern uint16_t* memsetw(uint16_t* dst, uint16_t val, int32_t cnt);
// extern int32_t strlen(const int8_t* str);
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t data);


#define HALT() __asm__ __volatile__("hlt")
#define CLI() __asm__ __volatile__("cli")
#define STI() __asm__ __volatile__("sti")

static inline size_t strlen(const char *str) {
    size_t l = 0;
    while (*str++ != 0) {
        l++;
    }
    return l;
}

static inline void *memset(void *ptr, int value, size_t num) {
    uint8_t *p = (uint8_t *)ptr;
    for (size_t i = 0; i < num; i++) {
        p[i] = value;
    }
    return ptr;
}

static inline void *memcpy(void *dest, const void *src, size_t num) {
    uint8_t *d = (uint8_t *)dest;
    uint8_t *s = (uint8_t *)src;
    for (size_t i = 0; i < num; i++) {
        d[i] = s[i];
    }
    return dest;
}

static inline int strcmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

static inline char *itoa(int value, char *str, int base) {
    char *rc;
    char *ptr;
    char *low;
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    if (value < 0 && base == 10) {
        *ptr++ = '-';
    }
    low = ptr;
    do {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    *ptr-- = '\0';
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

static inline char *strcat(char *dest, const char *src) {
    char *ptr = dest;
    while (*ptr) {
        ptr++;
    }
    while (*src) {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}

static inline char *strcpy(char *dest, const char *src) {
    char *ptr = dest;
    while (*src) {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}

static inline char *strchr(const char *str, int c) {
    while (*str) {
        if (*str == c) {
            return (char *)str;
        }
        str++;
    }
    return NULL;
}

static inline char *strstr(const char *haystack, const char *needle) {
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && *h == *n) {
            h++;
            n++;
        }
        if (!*n) {
            return (char *)haystack;
        }
        haystack++;
    }
    return NULL;
}

static inline int strncmp(const char *str1, const char *str2, size_t num) {
    while (num--) {
        if (*str1 != *str2) {
            return *(unsigned char *)str1 - *(unsigned char *)str2;
        }
        if (*str1 == '\0') {
            return 0;
        }
        str1++;
        str2++;
    }
    return 0;
}

static inline size_t strcspn(const char *str1, const char *str2) {
    size_t len = 0;
    while (*str1) {
        if (strchr(str2, *str1)) {
            return len;
        }
        str1++;
        len++;
    }
    return len;
}

static inline bool isequal(uint8_t *a, uint8_t *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}
#endif /* __SYS_H__ */
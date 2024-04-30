#ifndef UTIL_H
#define UTIL_H

// fixed width integer types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef u32 size_t;
typedef u32 uintptr_t;
typedef float f32;
typedef double f64;

typedef u8 bool;
#define true (1)
#define false (0)

#define NULL (0)

#define PACKED __attribute__((packed))

// #ifndef asm
// #define asm __asm__ volatile
// #endif

#define cli() asm ("cli")
#define sti() asm ("sti")
#define hlt() asm ("hlt")

static inline u16 inports(u16 port) {
    u16 r;
    asm("inw %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outports(u16 port, u16 data) {
    asm("outw %1, %0" : : "dN" (port), "a" (data));
}

static inline u8 inportb(u16 port) {
    u8 r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outportb(u16 port, u8 data) {
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

static inline size_t strlen(const char *str) {
    size_t l = 0;
    while (*str++ != 0) {
        l++;
    }
    return l;
}

static inline void *memset(void *ptr, int value, size_t num) {
    u8 *p = (u8 *)ptr;
    for (size_t i = 0; i < num; i++) {
        p[i] = value;
    }
    return ptr;
}

static inline void *memcpy(void *dest, const void *src, size_t num) {
    u8 *d = (u8 *)dest;
    u8 *s = (u8 *)src;
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

static inline bool isequal(u8 *a, u8 *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

#endif // UTIL_H

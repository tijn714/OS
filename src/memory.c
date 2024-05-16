#include "memory.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char* itoa(int value, char* str, int base) {
    char* rc;
    char* ptr;
    char* low;
    // Check for supported base.
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if (value < 0 && base == 10) {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void* memset(void* ptr, int value, size_t num) {
    unsigned char* ptr_ = (unsigned char*)ptr;
    while (num--) {
        *ptr_++ = (unsigned char)value;
    }
    return ptr;
}

void* memcpy(void* dest, const void* src, size_t num) {
    unsigned char* dest_ = (unsigned char*)dest;
    const unsigned char* src_ = (const unsigned char*)src;
    while (num--) {
        *dest_++ = *src_++;
    }
    return dest;
}

char* strstr(const char* haystack, const char* needle) {
    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return (char*)haystack;
    }
    size_t haystack_len = strlen(haystack);
    for (size_t i = 0; i < haystack_len; i++) {
        if (haystack[i] == needle[0]) {
            if (needle_len == 1) {
                return (char*)&haystack[i];
            }
            if (i + needle_len > haystack_len) {
                return NULL;
            }
            if (isEqual((const uint8_t*)&haystack[i], (const uint8_t*)needle, needle_len)) {
                return (char*)&haystack[i];
            }
        }
    }
    return NULL;
}

bool isEqual(const uint8_t* a, const uint8_t* b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool islower(char c) {
    return c >= 'a' && c <= 'z';
}

bool isupper(char c) {
    return c >= 'A' && c <= 'Z';
}

bool isint(char c) {
    return c >= '0' && c <= '9';
}

bool isdigit(char c) {
    return c >= '0' && c <= '9';
}
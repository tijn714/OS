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

int strncmp(const char* str1, const char* str2, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

int strcmp(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    if (len1 != len2) {
        return len1 - len2;
    }
    return strncmp(str1, str2, len1);
}

bool isEqual(const uint8_t* a, const uint8_t* b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool isint(char c) {
    return c >= '0' && c <= '9';
}

int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int isalpha(char c) {
    return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')));
}

char upper(char c) {
    if ((c >= 'a') && (c <= 'z'))
        return (c - 32);
    return c;
}

char lower(char c) {
    if ((c >= 'A') && (c <= 'Z'))
        return (c + 32);
    return c;
}


void clear_buffer(char *buffer) {
    // get the length of the buffer
    int len = strlen(buffer);

    // set the last character to null
    buffer[len - 1] = '\0';

    // set the rest of the buffer to null
    for (int i = 0; i < len; i++) {
        buffer[i] = '\0';
    }
}

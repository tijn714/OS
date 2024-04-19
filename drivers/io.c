#include "io.h"

uint8_t inb(uint16_t port) {
    uint8_t data;
    asm volatile("inb %1, %0" : "=a" (data) : "d" (port));
    return data;
}

void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a" (data), "d" (port));
}

uint16_t inw(uint16_t port) {
    uint16_t data;
    asm volatile("inw %1, %0" : "=a" (data) : "d" (port));
    return data;
}

void outw(uint16_t port, uint16_t data) {
    asm volatile("outw %0, %1" : : "a" (data), "d" (port));
}

uint32_t ind(uint16_t port) {
    uint32_t data;
    asm volatile("inl %1, %0" : "=a" (data) : "d" (port));
    return data;
}

void outd(uint16_t port, uint32_t data) {
    asm volatile("outl %0, %1" : : "a" (data), "d" (port));
}

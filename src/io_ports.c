#include "io_ports.h"

// Read a byte from the specified port
uint8_t inportb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Write a byte to the specified port
void outportb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}
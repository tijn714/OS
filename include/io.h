#ifndef IO_PORTS_H
#define IO_PORTS_H

#include "types.h"

// Read a byte from an I/O port
uint8_t inb(uint16_t port);

// Write a byte to an I/O port
void outb(uint16_t port, uint8_t data);

// Read a word from an I/O port
uint16_t inw(uint16_t port);

// Write a word to an I/O port
void outw(uint16_t port, uint16_t data);

// Read a double word from an I/O port
uint32_t ind(uint16_t port);

// Write a double word to an I/O port
void outd(uint16_t port, uint32_t data);

#endif // IO_PORTS_H
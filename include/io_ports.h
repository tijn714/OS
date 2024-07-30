#ifndef IO_PORTS_H
#define IO_PORTS_H

#include "types.h"

// Read a byte from the specified port
uint8_t inportb(uint16_t port);

// Write a byte to the specified port
void outportb(uint16_t port, uint8_t data);


// outportw
void outportw(uint16_t port, uint16_t data);

// inportw
uint16_t inportw(uint16_t port);
#endif // IO_PORTS_H

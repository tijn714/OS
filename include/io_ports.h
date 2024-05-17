#ifndef IO_PORTS_H
#define IO_PORTS_H

#include "types.h"

// Read a byte from the specified port
uint8_t inportb(uint16_t port);

// Write a byte to the specified port
void outportb(uint16_t port, uint8_t data);


// in_byte and out_byte are the same as inportb and outportb
#define in_byte inportb
#define out_byte outportb

#endif // IO_PORTS_H

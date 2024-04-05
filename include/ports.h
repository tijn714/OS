#ifndef PORTS_H
#define PORTS_H

#include "types.h"

byte inb(word port);
void outb(word port, byte data);

byte inb(word port)
{
    byte result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(word port, byte data)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(port));
}

#endif // PORTS_H
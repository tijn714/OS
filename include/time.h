#ifndef TIME_H
#define TIME_H

#include "types.h"
#include "ports.h"

uint32_t timer_ticks = 0;

void timer_callback()
{
    timer_ticks++;
}

void init_timer(uint32_t freq);
void sleep(uint32_t ms);

void init_timer(uint32_t freq)
{
    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

void sleep(uint32_t ms)
{
    uint32_t ticks = ms / 10;
    uint32_t start = timer_ticks;
    while (timer_ticks < start + ticks);
}

#endif // TIME_H
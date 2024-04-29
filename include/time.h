#ifndef TIME_H
#define TIME_H

#include "types.h"
#include "io.h"
#include "idt.h"
#include "isr.h"
#include "mem.h"

#define TIMER_INPUT_CLOCK_FREQUENCY    1193180
#define TIMER_CHANNEL_0_DATA_PORT    0x40
#define TIMER_CHANNEL_1_DATA_PORT    0x41
#define TIMER_CHANNEL_2_DATA_PORT    0x42
#define TIMER_COMMAND_PORT    0x43

// define timer_handler
void timer_handler(REGISTERS *reg);

struct TimerBlock {
    uint32_t ticks;
    uint32_t countdown;
};

void set_timer_frequency(uint32_t frequency);
void timer_init();
void sleep(uint32_t delay);
void set_date_time(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint32_t year);

#endif // TIME_H
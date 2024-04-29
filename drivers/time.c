#include "time.h"

uint32_t timer_ticks = 0;

void timer_handler(REGISTERS *reg) {
    timer_ticks++;
}

// set the timer frequency
void set_timer_frequency(uint32_t frequency) {
    uint32_t divisor = TIMER_INPUT_CLOCK_FREQUENCY / frequency;
    outb(TIMER_COMMAND_PORT, 0x36);
    outb(TIMER_CHANNEL_0_DATA_PORT, divisor & 0xFF);
    outb(TIMER_CHANNEL_0_DATA_PORT, (divisor >> 8) & 0xFF);

    isr_register_interrupt_handler(32, timer_handler);
}

void timer_init() {
    set_timer_frequency(100);
}

void sleep(uint32_t delay) {
    uint32_t start = timer_ticks;
    while (timer_ticks < start + delay);
}
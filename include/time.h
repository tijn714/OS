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

typedef struct {
    uint32_t timeout; // in millisecond, g_ticks in timer.c reaches there
    void *user;
} TIMER_FUNC_ARGS;

typedef void (*TIMER_FUNCTION)(TIMER_FUNC_ARGS *);

#define MAXIMUM_TIMER_FUNCTIONS    32

typedef struct {
    uint32_t current_index;
    // timer functions to be called when that ticks reached in irq handler
    TIMER_FUNCTION functions[MAXIMUM_TIMER_FUNCTIONS];
    // arguments of each above timer functions
    TIMER_FUNC_ARGS func_args[MAXIMUM_TIMER_FUNCTIONS];
} TIMER_FUNCTION_MANAGER;


void timer_init();
void sleep(int sec);

void timer_register_function(TIMER_FUNCTION function, TIMER_FUNC_ARGS *args);

#endif // TIME_H
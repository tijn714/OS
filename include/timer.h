#ifndef TIMER_H
#define TIMER_H

#include "types.h"

#define TIMER_INPUT_CLOCK_FREQUENCY    1193180
#define TIMER_CHANNEL_0_DATA_PORT    0x40
#define TIMER_CHANNEL_1_DATA_PORT    0x41
#define TIMER_CHANNEL_2_DATA_PORT    0x42
#define TIMER_COMMAND_PORT    0x43

typedef struct {
    uint32_t timeout; // in millisecond, g_ticks in timer.c reaches there
    void *user;
} TIMER_FUNC_ARGS;


// date and time structure
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
} DATE_TIME;


// define the range of each field in date and time
#define MAX_SECOND    59
#define MAX_MINUTE    59
#define MAX_HOUR    23
#define MAX_DAY    31
#define MAX_MONTH    12
#define MAX_YEAR_DIGIT    4

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
void print_sys_uptime();
void get_date_time(DATE_TIME *dt);

#endif

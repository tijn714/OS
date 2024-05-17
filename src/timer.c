#include "timer.h"

#include "vga.h"
#include "idt.h"
#include "io_ports.h"
#include "isr.h"
#include "memory.h"

uint32_t g_ticks = 0;
uint16_t g_freq_hz = 0;
TIMER_FUNCTION_MANAGER g_timer_function_manager;

void timer_set_frequency(uint16_t f) {
    g_freq_hz = f;
    uint16_t divisor = TIMER_INPUT_CLOCK_FREQUENCY / f;
    // set Mode 3 - Square Wave Mode
    outportb(TIMER_COMMAND_PORT, 0b00110110);
    // set low byte
    outportb(TIMER_CHANNEL_0_DATA_PORT, divisor & 0xFF);
    // set high byte
    outportb(TIMER_CHANNEL_0_DATA_PORT, (divisor >> 8) & 0xFF);
}


void timer_handler(REGISTERS* r) {
    uint32_t i;
    TIMER_FUNC_ARGS *args = NULL;
    g_ticks++;
    //kprint("timer triggered at frequency %d\n", g_ticks);
    for (i = 0; i < MAXIMUM_TIMER_FUNCTIONS; i++) {
        args = &g_timer_function_manager.func_args[i];
        if (args->timeout == 0)
            continue;
        if ((g_ticks % args->timeout) == 0) {
            g_timer_function_manager.functions[i](args);
        }
    }
}

void timer_register_function(TIMER_FUNCTION function, TIMER_FUNC_ARGS *args) {
    uint32_t index = 0;
    if (function == NULL || args == NULL) {
        kprint(" ERROR: failed to register timer function %x\n", function);
        return;
    }
    index = (++g_timer_function_manager.current_index) % MAXIMUM_TIMER_FUNCTIONS;
    g_timer_function_manager.current_index = index;
    g_timer_function_manager.functions[index] = function;
    memcpy(&g_timer_function_manager.func_args[index], args, sizeof(TIMER_FUNC_ARGS));
}

void timer_init() {
    // IRQ0 will fire 100 times per second
    timer_set_frequency(100);
    isr_register_interrupt_handler(IRQ_BASE, timer_handler);

    kserial(GREEN, LIGHT_GREEN, " [OK] Timer initialized\n");
}

void sleep(int sec) {
    uint32_t end = g_ticks + sec * g_freq_hz;
    while (g_ticks < end);
}

void get_date_time(DATE_TIME *dt) {
    // read RTC registers
    outportb(0x70, 0x00);
    dt->second = inportb(0x71);
    outportb(0x70, 0x02);
    dt->minute = inportb(0x71);
    outportb(0x70, 0x04);
    dt->hour = inportb(0x71);
    outportb(0x70, 0x07);
    dt->day = inportb(0x71);
    outportb(0x70, 0x08);
    dt->month = inportb(0x71);
    outportb(0x70, 0x09);
    dt->year = inportb(0x71);

    // convert BCD to binary
    dt->second = (dt->second & 0x0F) + ((dt->second / 16) * 10);
    dt->minute = (dt->minute & 0x0F) + ((dt->minute / 16) * 10);
    dt->hour = (dt->hour & 0x0F) + (((dt->hour & 0x70) / 16) * 10);
    dt->day = (dt->day & 0x0F) + ((dt->day / 16) * 10);
    dt->month = (dt->month & 0x0F) + ((dt->month / 16) * 10);
    dt->year = (dt->year & 0x0F) + ((dt->year / 16) * 10);


    // add 2 hours to adjust for timezone
    dt->hour += 2;

    // convert 12-hour clock to 24-hour clock
    if (!(dt->hour & 0x80)) {
        if ((dt->hour & 0x70) == 0x70) {
            dt->hour = 0x10 | (dt->hour & 0x0F);
        }
    } else {
        if ((dt->hour & 0x70) != 0x70) {
            dt->hour = 0x20 | (dt->hour & 0x0F);
        }
    }

    // convert year to 4-digit format
    dt->year += 2000;

    kprint(" Booted: %d-%d-%d on %d:%d\n", dt->day, dt->month, dt->year, dt->hour, dt->minute);

}
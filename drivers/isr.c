#include "isr.h"
#include "idt.h"
#include "pic.h"
#include "vga.h"
#include "timer.h"

// For both exceptions and irq interrupt
ISR g_interrupt_handlers[NO_INTERRUPT_HANDLERS];

char *exception_messages[32] = {
    "0xFF00 - Delen door nul",
    "0xFD01 - Debug",
    "0xFA02 - Niet-maskerbare interrupt",
    "0xBF03 - Breakpoint",
    "0x0F04 - Overloop",
    "0xBD05 - BOUND-bereik overschreden",
    "0x0D06 - Ongeldige opcode",
    "0xDE07 - Apparaat niet beschikbaar (geen math-coprocessor)",
    "0x0F08 - Dubbele fout",
    "0x0F09 - Coprocessor segment overschreden",
    "0x0F0A - Ongeldige TSS",
    "0x0F0B - Segment niet aanwezig",
    "0x0F0C - Stack-segmentfout",
    "0x0F0D - Algemene bescherming",
    "0x0F0E - Paginafout",
    "0x0F0F - Onbekende interruptie (Intel gereserveerd)",
    "0x0F10 - x87 FPU zwevend-punt fout (Math-fout)",
    "0x0F11 - Alignementcontrole",
    "0x0F12 - Machinestoring",
    "0x0F13 - SIMD zwevend-punt uitzondering",
    "0x0F14 - Virtualisatie-uitzondering",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd",
    "0x00FF - Gereserveerd"
};

void isr_register_interrupt_handler(int num, ISR handler) {
    if (num < NO_INTERRUPT_HANDLERS)
        g_interrupt_handlers[num] = handler;
}

void isr_end_interrupt(int num) {
    pic8259_eoi(num);
}

void isr_irq_handler(REGISTERS *reg) {
    if (g_interrupt_handlers[reg->int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg->int_no];
        handler(reg);
    }
    pic8259_eoi(reg->int_no);
}

void isr_exception_handler(REGISTERS reg) {
    if (reg.int_no < 32) {
        set_color(WHITE, BLUE);
        clear_screen();
        kprint("\n\n");
        kprint(" :( \n\n Er is een fout opgetreden\n\n");
        kprint(" Foutcode: %s\n", exception_messages[reg.int_no]);

        kprint("\n\n Als je dit scherm ziet, is er iets misgegaan. Herstart uw computer en probeer \n");
        kprint(" het opnieuw.\n");

        disable_cursor();
    }
    if (g_interrupt_handlers[reg.int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg.int_no];
        handler(&reg);
    }
}

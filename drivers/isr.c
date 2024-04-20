#include "isr.h"

static struct registers* current_regs = NULL;

void isr_handler(struct registers regs) {
    current_regs = &regs;
    switch (regs.int_no) {
        case 0:
            isr_0_handler();
            break;
        case 1:
            isr_1_handler();
            break;
        case 2:
            isr_2_handler();
            break;
        case 3:
            isr_3_handler();
            break;
        case 4:
            isr_4_handler();
            break;
        case 5:
            isr_5_handler();
            break;
        case 6:
            isr_6_handler();
            break;
        case 7:
            isr_7_handler();
            break;
        case 8:
            isr_8_handler();
            break;
        case 9:
            isr_9_handler();
            break;
        case 10:
            isr_10_handler();
            break;
        case 11:
            isr_11_handler();
            break;
        case 12:
            isr_12_handler();
            break;
        case 13:
            isr_13_handler();
            break;
        case 14:
            isr_14_handler();
            break;
        case 15:
            isr_15_handler();
            break;
    }
}

void isr_0_handler() {
    kprint("Division by zero exception\n");
    for(;;);
}

void isr_1_handler() {
    kprint("Debug exception\n");
    for(;;);
}

void isr_2_handler() {
    kprint("Non-maskable interrupt\n");
    for(;;);
}

void isr_3_handler() {
    kprint("Breakpoint exception\n");
    for(;;);
}

void isr_4_handler() {
    kprint("Overflow exception\n");
    for(;;);
}

void isr_5_handler() {
    kprint("Bound range exceeded exception\n");
    for(;;);
}

void isr_6_handler() {
    kprint("Invalid opcode exception\n");
    for(;;);
}

void isr_7_handler() {
    kprint("Device not available exception\n");
    for(;;);
}

void isr_8_handler() {
    kprint("Double fault exception\n");
    for(;;);
}

void isr_9_handler() {
    kprint("Coprocessor segment overrun exception\n");
    for(;;);
}

void isr_10_handler() {
    kprint("Invalid TSS exception\n");
    for(;;);
}

void isr_11_handler() {
    kprint("Segment not present exception\n");
    for(;;);
}

void isr_12_handler() {
    kprint("Stack fault exception\n");
    for(;;);
}

void isr_13_handler() {
    kprint("General protection fault exception\n");
    for(;;);
}

void isr_14_handler() {
    kprint("Page fault exception\n");
    for(;;);
}

void isr_15_handler() {
    kprint("Unknown interrupt exception\n");
    for(;;);
}

void common_isr_stub_handler() {
    kprint("Common ISR stub handler\n");
    for(;;);
}

void isr_0() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_1() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_2() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_3() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_4() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_5() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_6() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_7() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_8() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_9() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_10() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_11() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_12() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_13() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_14() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

void isr_15() {
    asm volatile("pusha");
    isr_handler(*current_regs);
    asm volatile("popa; leave; iret");
}

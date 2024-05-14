#include "system.h"
#include "gdt.h"
#include "idt.h"
#include "isrs.h"
#include "irq.h"
#include "timer.h"
#include "vga.h"

void kmain(void) {
    gdt_init();
    idt_init();
    irq_init();
    timer_init();
    vga_init();

    kprint("Hello, World!\n Привет мир! \n™ © ¥\n\n");
    print_ticks();
} 
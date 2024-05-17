#include "memory.h"
#include "vga.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include "io_ports.h"
#include "timer.h"


void kmain(void) {
    vga_init();
    gdt_init();
    idt_init();
    timer_init();

    set_color(WHITE, BLUE);
    clear_screen();
    DATE_TIME dt;
    get_date_time(&dt);

    kprint("\n ");
    for (int i = 0; i < 16; i++) {
        kserial(WHITE, i, "  ");
    }
    kprint("\n");
    

    kprint(" OS version 0.0.1\n");


    // countdown
    for (int i = 3; i > 0; i--) {
        kprint("\r Starting in %d seconds...", i);
        sleep(1);
    }
    kprint("\n");

    // cause TSS exception
    __asm__ __volatile__("int $0x30");

} 
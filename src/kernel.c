#include "memory.h"
#include "vga.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include "io_ports.h"


void kmain(void) {
    vga_init();

    kprint(" OS v0.00000000000000001");
    kprint("\n ");
    for (int i = 0; i < 16; i++) {
        kserial(WHITE, i, "  ");
    }
    kprint("\n\n");

    gdt_init();
    idt_init();


} 
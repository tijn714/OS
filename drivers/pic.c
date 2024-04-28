#include "pic.h"

void pic8259_init() {
    uint8_t a1, a2;

    // save mask registers
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    // send commands to pic to initialize both master & slave
    outb(PIC1_COMMAND, ICW1);
    outb(PIC2_COMMAND, ICW1);

    // map vector offset of all default IRQ's from 0x20 to 0x27 in master(ICW2)
    outb(PIC1_DATA, 0x20);
    // map vector offset of all default IRQ's from 0x28 to 0x2F in slave(ICW2)
    outb(PIC2_DATA, 0x28);

    // ICW3: tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC1_DATA, 4);
    // ICW3: tell slave PIC its cascade identity (0000 0010)
    outb(PIC2_DATA, 2);

    // ICW4, set x86 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // restore the mask registers
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic8259_eoi(uint8_t irq) {
    if(irq >= 0x28)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

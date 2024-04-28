#include "irq.h"

static void (*irq_handlers[16])(struct registers);

void irq0() {
    irq0_handler();
}

void irq1() {
    irq1_handler();
}

// Install IRQ handler
void irq_install_handler(int irq, void (*handler)(struct registers)) {
    irq_handlers[irq] = handler;
    kprint(" Installed IRQ handler for IRQ%d\n", irq);
}

// Uninstall IRQ handler
void irq_uninstall_handler(int irq) {
    irq_handlers[irq] = NULL;
    kprint(" Uninstalled IRQ handler for IRQ%d\n", irq);
}

// Remap IRQs
void irq_remap(void) {
    // Send ICW1 to master and slave PIC
    outb(0x20, 0x11); // Start initialization sequence for master PIC
    outb(0xA0, 0x11); // Start initialization sequence for slave PIC

    // Send ICW2: IRQ base offsets
    outb(0x21, 0x20); // Set master PIC IRQ base offset to 0x20 (32)
    outb(0xA1, 0x28); // Set slave PIC IRQ base offset to 0x28 (40)

    // Send ICW3: Configure master and slave PICs
    outb(0x21, 0x04); // Tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(0xA1, 0x02); // Tell slave PIC its cascade identity (0000 0010)

    // Send ICW4: Set mode
    outb(0x21, 0x01); // Set master PIC to 8086 mode
    outb(0xA1, 0x01); // Set slave PIC to 8086 mode

    // Mask all IRQs
    outb(0x21, 0xFF); // Mask all interrupts on master PIC
    outb(0xA1, 0xFF); // Mask all interrupts on slave PIC
}

// Install IRQs
void irq_install(void) {
    // Remap PIC
    irq_remap();

    // Install IRQ handlers
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E); // IRQ0 - Timer
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E); // IRQ1 - Keyboard
    // Add more IRQs as needed
}

// IRQ0 handler (Timer)
void irq0_handler(struct registers regs) {
    // Send end of interrupt signal to PIC
    outb(0x20, 0x20);

    // Call IRQ handler if installed
    if (irq_handlers[0] != NULL) {
        irq_handlers[0](regs);
    }
}

// IRQ1 handler (Keyboard)
void irq1_handler(struct registers regs) {
    // Send end of interrupt signal to PIC
    outb(0x20, 0x20);

    // Call IRQ handler if installed
    if (irq_handlers[1] != NULL) {
        irq_handlers[1](regs);
    }
}

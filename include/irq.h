#ifndef IRQ_H
#define IRQ_H

#include "types.h"
#include "isr.h"
#include "idt.h"
#include "io.h"
#include "vga.h"

void irq0();
void irq1();

void irq_install_handler(int irq, void (*handler)(struct registers));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_install(void);

#endif // IRQ_H
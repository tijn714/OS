#ifndef CPU_H
#define CPU_H

#include "types.h"
#include "vga.h"

int print_cpu_info(void);
void halt_cpu();


int print_cpu_info(void)
{
   // TODO
   return 0;
}


void halt_cpu() {
    __asm__ __volatile__("hlt");
}

void reboot() {
    outb(0x64, 0xFE);
}


#endif // CPU_H
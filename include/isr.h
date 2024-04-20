#ifndef ISR_H
#define ISR_H

#include "types.h"
#include "vga.h"

struct registers
{
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by pusha
  uint32_t int_no, err_code;  // interrupt number and error code
  uint32_t eip, cs, eflags, useresp, ss;  // pushed by the processor automatically
};

void isr_0();
void isr_1();
void isr_2();
void isr_3();
void isr_4();
void isr_5();
void isr_6();
void isr_7();
void isr_8();
void isr_9();
void isr_10();
void isr_11();
void isr_12();
void isr_13();
void isr_14();
void isr_15();

void common_isr_stub_handler();
void isr_handler(struct registers);

void isr_0_handler();
void isr_1_handler();
void isr_2_handler();
void isr_3_handler();
void isr_4_handler();
void isr_5_handler();
void isr_6_handler();
void isr_7_handler();
void isr_8_handler();
void isr_9_handler();
void isr_10_handler();
void isr_11_handler();
void isr_12_handler();
void isr_13_handler();
void isr_14_handler();
void isr_15_handler();

#endif
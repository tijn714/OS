#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "io.h"
#include "isr.h"
#include "mem.h"

#include "vga.h"

// Define the structure of an entry in the IDT
struct idt_entry {
    uint16_t base_low;
    uint16_t segment_selector;
    uint8_t zero;
    uint8_t type;
    uint16_t base_high;
} __attribute__((packed));

// Define the pointer to the IDT
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Declare the IDT entry table and pointer
extern struct idt_entry idt_entries[256];
extern struct idt_ptr idt_ptr;

// Function prototypes
extern void idt_load();
void idt_init();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t type);

#endif // IDT_H

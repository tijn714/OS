#ifndef GDT_H
#define GDT_H

#include "types.h"
#include "vga.h"

#define NULL_SEGMENT 0
#define CODE_SEGMENT 1
#define DATA_SEGMENT 2

// Define the structure of a GDT entry
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// Define the structure of a GDT pointer
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Declare the GDT entry table and pointer
extern struct gdt_entry gdt_entries[3];
extern struct gdt_ptr gdt_ptr;

// Function prototypes
void gdt_init();
extern void gdt_load(); // defined in load_gdt.asm
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);


#endif // GDT_H
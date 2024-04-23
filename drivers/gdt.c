#include "gdt.h"

// Define the GDT entry table and pointer
struct gdt_entry gdt_entries[3];
struct gdt_ptr gdt_ptr;

extern void gdt_load(); // defined in load_gdt.asm

// Initialize the GDT
void gdt_init() {
    // Set up the GDT pointer and limit
    gdt_ptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Set up the null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // Set up the code segment descriptor
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Set up the data segment descriptor
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT
    gdt_load();
}

// Set up a GDT gate
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Set up the base address
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    // Set up the limit
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    // Set up the granularity and access flags
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}
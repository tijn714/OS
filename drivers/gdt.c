#include "gdt.h"

// Define the GDT entry table and pointer
struct gdt_entry gdt_entries[3];
struct gdt_ptr gdt_ptr;

// Initialize the GDT
void gdt_init() {
    // Set up the GDT pointer and limit
    gdt_ptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Set up the null segment
    gdt_set_gate(NULL_SEGMENT, 0, 0, 0, 0);

    // Set up the code segment
    gdt_set_gate(CODE_SEGMENT, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Set up the data segment
    gdt_set_gate(DATA_SEGMENT, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT
    gdt_load();

    print_ok("GDT initialized");
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

// Load the GDT
void gdt_load() {
    // Load the GDT
    asm volatile("lgdt %0" : : "m" (gdt_ptr));

    // Reload the segment registers using far jump with a far pointer
    asm volatile(
        "mov $0x10, %ax\n"
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        "mov %ax, %ss\n"
        "ljmp $0x08, $1f\n"  // Far jump to the label 1f
        "1:\n"                // Define the label 1
    );
}

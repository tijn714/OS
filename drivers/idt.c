#include "idt.h"

// Define the IDT entry table and pointer
struct idt_entry idt_entries[256];
struct idt_ptr idt_ptr;

void idt_load() {
    asm volatile("lidt %0" : : "m" (idt_ptr));
}

void idt_init() {
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    memset((uint8_t *)idt_entries, 0, sizeof(struct idt_entry) * 256);

    idt_load();

    print_ok("IDT initialized");
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t type) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].segment_selector = selector;
    idt_entries[num].zero = 0;
    idt_entries[num].type = type;
}

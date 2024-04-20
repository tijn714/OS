#include "types.h"
#include "mem.h"
#include "vga.h"
#include "io.h"
#include "idt.h"
#include "gdt.h"
#include "fat.h" // In Development

void __cpuid(uint32_t type, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile("cpuid"
                : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                : "0"(type)); // put the type into eax
}

void print_memory_info(void) {
    uint32_t eax, ebx, ecx, edx;
    __cpuid(0x80000005, &eax, &ebx, &ecx, &edx);
    kprint("L1 Cache: %d KB\n", (eax & 0xff) / 1024);
    kprint("L2 Cache: %d KB\n", (eax >> 8 & 0xff) / 1024);
    kprint("L3 Cache: %d KB\n", (eax >> 16 & 0xff) / 1024);
}

void print_ram_info(void) {
    uint32_t eax, ebx, ecx, edx;
    __cpuid(0x80000005, &eax, &ebx, &ecx, &edx);
    kprint("RAM: %d GB\n", (ebx >> 8 & 0xff) / 1024);
}

void kmain(void) {
    vga_init();
    idt_init();
    gdt_init();

    print_info("Copyright (c) 2024 Tijn Rodrigo - All Rights Reserved\n");
    uint32_t brand[12];
    uint32_t eax, ebx, ecx, edx;
    uint32_t type;

    // set memory to 0
    memset(brand, 0, 12);

    // get vendor
    __cpuid(0x80000002, (uint32_t *)brand+0x0, (uint32_t *)brand+0x1, (uint32_t *)brand+0x2, (uint32_t *)brand+0x3);
    __cpuid(0x80000003, (uint32_t *)brand+0x4, (uint32_t *)brand+0x5, (uint32_t *)brand+0x6, (uint32_t *)brand+0x7);
    __cpuid(0x80000004, (uint32_t *)brand+0x8, (uint32_t *)brand+0x9, (uint32_t *)brand+0xa, (uint32_t *)brand+0xb);

    // print vendor
    kprint(" Detected CPU: %s", brand);

    // check if the cpu is 32 or 64 bit
    __cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
    if (edx & (1 << 29)) {
        kprint(" (64-bit) \n");
    } else {
        kprint(" (32-bit) \n");
    }

    // print memory info
    print_memory_info();

    // print ram info
    print_ram_info();
}
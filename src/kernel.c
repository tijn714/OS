#include "types.h"
#include "mem.h"
#include "vga.h"
#include "io.h"
#include "idt.h"
#include "gdt.h"
#include "aes.h"
#include "base64.h"

void __cpuid(uint32_t type, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile("cpuid"
                : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                : "0"(type)); // put the type into eax
}

void kmain(void) {
    vga_init();
    idt_init();
    gdt_init();

    print_info("Copyright (c) 2024 Tijn Rodrigo - All Rights Reserved.");
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

    uint8_t in[16] = {
        0xDB, 0xDB, 0xDB, 0xDB,
        0xDB, 0xDB, 0xDB, 0xDB,
        0xDB, 0xDB, 0xDB, 0xDB,
        0xDB, 0xDB, 0xDB, 0xDB
    };
    
    uint8_t key[32] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c,
        0xa0, 0xfa, 0xfe, 0x17,
        0x88, 0x54, 0x2c, 0xb1,
        0x23, 0xa3, 0x39, 0x39,
        0x2a, 0x6c, 0x76, 0x05
    };

    uint8_t RoundKey[240];
    uint8_t encrypted[16];
    uint8_t decrypted[16];

    // base64 encode the encrypted block
    char encoded[24];
    base64_encode(in, 16, encoded);

    // decode the base64 encoded block
    uint8_t decoded[16];
    base64_decode(encoded, 24, decoded);

    // check if the input and decoded block are the same
    if (isEqual(in, decoded, 16)) {
        print_ok(" Base64 test passed");
    } else {
        print_fail(" Base64 test failed!");
    }


    KeyExpansion(key, RoundKey);

    AES_Encrypt(in, encrypted, RoundKey);
    AES_Decrypt(encrypted, decrypted, RoundKey);

    // check if the input and decrypted block are the same
    if (isEqual(in, decrypted, 16)) {
        print_ok(" AES test passed");
    } else {
        print_fail(" AES test failed!");
    }
}
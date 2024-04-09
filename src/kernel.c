#include "types.h"
#include "ports.h"
#include "memory.h"
#include "vga.h"

void kernelmain() {
    vga_init();
    vga_clear();

    char *str = "Hello, World!";
    int len = strlen(str);
    vga_print(" String: %s\n Length: %d\n", str, len);

    outportb(0x64, 0xFE);
}
#include "vga.h"

void kmain(void) {
    vga_init();
    clear_screen();

    kprint("\n\n");
    kserial(WHITE, BLUE, " OS - v0.01 \n");
    kserial(WHITE, BLUE, " Dit project is nog in de BETA VERSIE! het kan nog niet werken als verwacht!\n");

    // use a for loop and kputchar to print all background colors in a row
    kprint("\n ");
    for (int i = 0; i < 16; i++) {
        kputchar(' ', WHITE, i);
    }
    kprint("\n");
} 
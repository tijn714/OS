#include "types.h"
#include "vga.h"
#include "ports.h"
#include "mem.h"
#include "cpu.h"
#include "time.h"


void kernelmain(void) 
{
    vga_init();
    vga_clear();
    vga_print(" Welcome to my OS!\n");
}
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
    vga_print(" [ info ] Kernel loaded! \n");
    vga_print(" [ info ] VGA initialized! \n");
    vga_print(" [ info ] Memory initialized! \n");
}
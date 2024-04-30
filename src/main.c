#include "util.h"
#include "vga.h"

void kmain() {
    vga_init();
    clear_screen();

    kserial(LIGHT_RED, RED, " Voorbeeld van een Foutmelding!\n");
    kserial(LIGHT_GREEN, GREEN, " Voorbeeld van een Succesmelding!\n");
    kserial(LIGHT_BLUE, BLUE, " Voorbeeld van een Informatiemelding!\n\n");

    kserial(WHITE, LIGHT_GRAY, " Deze kernel is gemaakt door Tijn Rodrigo \n");
    kserial(WHITE, LIGHT_GRAY, " https://github.com/tijn714/OS \n\n");

    kprint(" Deze tekst wordt in de standaard kleuren weergegeven.\n");

    for(;;) {
        hlt();
    }
}
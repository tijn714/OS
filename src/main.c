#include "util.h"
#include "vga.h"

void loading_bar(int target, int current) {
    current++;
    int percent = (current * 100) / target;
    kprint("\r [");
    for (int i = 0; i < 50; i++) {
        if (i < percent / 2) {
            kprint("%c", 0xDB);
        } else {
            kprint(" ");
        }
    }
    kprint("] %d%%", percent);

    if (percent == 100) {
        kprint("\r [");
        for (int i = 0; i < 50; i++) {
            kprint("%c", 0xDB);
        }
        kprint("] %d%% - Done!\n", percent);
    }
}

void kmain() {
    vga_init();
    set_color(BLACK, LIGHT_GRAY);
    clear_screen();

    kserial(LIGHT_RED, RED, " Voorbeeld van een Foutmelding!\n");
    kserial(LIGHT_GREEN, GREEN, " Voorbeeld van een Succesmelding!\n");
    kserial(LIGHT_BLUE, BLUE, " Voorbeeld van een Informatiemelding!\n\n");

    kserial(WHITE, LIGHT_GRAY, " Deze kernel is gemaakt door Tijn Rodrigo \n");
    kserial(WHITE, LIGHT_GRAY, " https://github.com/tijn714/OS \n\n");

    kprint(" Deze tekst wordt in de standaard kleuren weergegeven.\n");


    u8 hex[16] = "Hallo, Wereld!  ";

    kprint(" Dit is de waarde van de hex: ");
    for (size_t i = 0; i < 16; i++) {
        kprint("%x ", hex[i]);
    }
    kprint("\n");

    long long num = 100000;
    kprint(" Dit is een getal: %d\n", num);

    for (int i = 0; i < num; i++) {
        loading_bar(num, i);
    }

    clear_screen();

    draw_information_box("Informatie", "Dit is een test", WHITE, BLACK);
}
#include "system.h"

void panic(const char *msg) {
    set_color(WHITE, BLUE);
    clear_screen();
    if (msg == NULL) {
        kprint(" PANIEK! PANIEK!\n De CPU is stop gezet om schade aan het systeem te voorkomen.\n Er is geen foutmelding beschikbaar.\n");
    } else {
        kprint(" PANIEK! PANIEK!\n De CPU is stop gezet om schade aan het systeem te voorkomen.\n Foutmelding: %s\n", msg);
    }
    hlt();
}
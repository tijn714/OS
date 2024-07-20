#include "memory.h"
#include "vga.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include "io_ports.h"
#include "timer.h"
#include "keyboard.h"
#include "cpu.h"


// void shell() {
//     char buffer[256];

//     char *shell = " shell@RodrigoX:~# ";

//     kserial(YELLOW, BLACK, " RodrigoX Shell\n");
//     DATE_TIME dt;
//     get_date_time(&dt);
//     format_date_time(&dt);

//     while (1) {
//         kserial(GREEN, BLACK, shell);
//         kb_getinput(buffer, 256);

//         kprint("\n");

//         if (buffer[0] == 0)
//             continue;

//         if (strcmp(buffer, "shutdown") == 0) { 
//             return;
//         } else if (strcmp(buffer, "reboot") == 0) {
//             return;
//         } else if (strcmp(buffer, "clear") == 0) {
//             clear_screen();
//         } else if (strcmp(buffer, "help") == 0) {
//             kprint(" Beschikbare commando's:\n");
//             kprint(" - shutdown\n");
//             kprint(" - reboot\n");
//             kprint(" - clear\n");
//             kprint(" - help\n");
//         } else {
//             kserial(RED, BLACK, " commando niet gevonden\n");
//         }
//     }
    
// }

void kmain() {
    gdt_init();
    idt_init();
    timer_init();
    keyboard_init();
    vga_video_init();

    draw_glyph(0, 0, WHITE, 'W');
    draw_glyph(8, 0, WHITE, 'e');
    draw_glyph(16, 0, WHITE, 'l');
    draw_glyph(24, 0, WHITE, 'c');
    draw_glyph(32, 0, WHITE, 'o');
    draw_glyph(40, 0, WHITE, 'm');
    draw_glyph(48, 0, WHITE, 'e');
    draw_glyph(56, 0, WHITE, ' ');
    draw_glyph(64, 0, WHITE, 't');
    draw_glyph(72, 0, WHITE, 'o');
    draw_glyph(80, 0, WHITE, ' ');
    draw_glyph(88, 0, WHITE, 'R');
    draw_glyph(96, 0, WHITE, 'o');
    draw_glyph(104, 0, WHITE, 'd');
    draw_glyph(112, 0, WHITE, 'r');
    draw_glyph(120, 0, WHITE, 'i');
    draw_glyph(128, 0, WHITE, 'g');
    draw_glyph(136, 0, WHITE, 'o');
    draw_glyph(144, 0, WHITE, 'X');
    draw_glyph(152, 0, WHITE, ' ');
    draw_glyph(160, 0, WHITE, 'O');
    draw_glyph(168, 0, WHITE, 'S');
    draw_glyph(176, 0, WHITE, '!');
}
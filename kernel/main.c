#include "types.h"
#include "memory.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include "io_ports.h"
#include "timer.h"
#include "keyboard.h"
#include "vga.h"

// void shell() {
//      DATE_TIME dt;
//     get_date_time(&dt);

//     kserial(GREEN, BLACK, " [info] Systeem opgestart op");


//     if (dt.day < 10) {
//         kserial(GREEN, BLACK, " 0%d", dt.day);
//     } else {
//         kserial(GREEN, BLACK, " %d", dt.day);
//     }

//     if (dt.month == 1) {
//         kserial(GREEN, BLACK, " jan");
//     } else if (dt.month == 2) {
//         kserial(GREEN, BLACK, " feb");
//     } else if (dt.month == 3) {
//         kserial(GREEN, BLACK, " mrt");
//     } else if (dt.month == 4) {
//         kserial(GREEN, BLACK, " apr");
//     } else if (dt.month == 5) {
//         kserial(GREEN, BLACK, " mei");
//     } else if (dt.month == 6) {
//         kserial(GREEN, BLACK, " jun");
//     } else if (dt.month == 7) {
//         kserial(GREEN, BLACK, " jul");
//     } else if (dt.month == 8) {
//         kserial(GREEN, BLACK, " aug");
//     } else if (dt.month == 9) {
//         kserial(GREEN, BLACK, " sep");
//     } else if (dt.month == 10) {
//         kserial(GREEN, BLACK, " okt");
//     } else if (dt.month == 11) {
//         kserial(GREEN, BLACK, " nov");
//     } else if (dt.month == 12) {
//         kserial(GREEN, BLACK, " dec");
//     } else {
//         if (dt.month < 10) {
//             kserial(GREEN, BLACK, " 0%d", dt.month);
//         } else {
//             kserial(GREEN, BLACK, " %d", dt.month);
//         }
//     }

//     kserial(GREEN, BLACK, " %d", dt.year);

//     kserial(GREEN, BLACK, " om");

//     if (dt.hour < 10) {
//         kserial(GREEN, BLACK, " 0%d", dt.hour);
//     } else {
//         kserial(GREEN, BLACK, " %d", dt.hour);
//     }

//     if (dt.minute < 10) {
//         kserial(GREEN, BLACK, ":0%d", dt.minute);
//     } else {
//         kserial(GREEN, BLACK, ":%d", dt.minute);
//     }

//     if (dt.second < 10) {
//         kserial(GREEN, BLACK, ":0%d", dt.second);
//     } else {
//         kserial(GREEN, BLACK, ":%d", dt.second);
//     }

//     kprint("\n\n");

//     char buffer[256];

//     while(1) {
//         kprint(" shell@RodrigoX~# ");
//         kb_getinput(buffer, 256);

//         kprint("\n");

//         if (strcmp(buffer, "") == 0) {
//             continue;
//         } 

//         if (strcmp(buffer, "clear") == 0) {
//             clear_screen();
//         } else if (strcmp(buffer, "uptime") == 0) {
            
//             int uptime_sec = u_ticks / 100;
//             int uptime_min = uptime_sec / 60;
//             int uptime_hour = uptime_min / 60;


//             kprint(" bedrijfstijd: ");

//             if (uptime_hour < 10) {
//                 kprint("0%d:", uptime_hour);
//             } else {
//                 kprint("%d:", uptime_hour);
//             }

//             if (uptime_min < 10) {
//                 kprint("0%d:", uptime_min % 60);
//             } else {
//                 kprint("%d:", uptime_min % 60);
//             }

//             if (uptime_sec < 10) {
//                 kprint("0%d\n", uptime_sec % 60);
//             } else {
//                 kprint("%d\n", uptime_sec % 60);
//             }
//         }
//         else if (strcmp(buffer, "help") == 0) {
//             if (vga_row > 19) {
//                 clear_screen();
//             }

//             kprint(" Beschikbare commando's:\n\n");
//             kprint(" - clear\n");
//             kprint(" - uptime\n");
//             kprint(" - help\n");
//         }
        
//         else {
//             kserial(RED, BLACK, " commando niet gevonden.\n");
//         }
//         clear_buffer(buffer);
//     }
// }


void kmain(void) {
    vga_init();
    gdt_init();
    idt_init();
    timer_init();
    keyboard_init();
    // vga_video_init();

    if (current_mode == TEXT) {
        kserial(RED, LIGHT_RED, " [info] VGA modus: Tekstmodus\n");
    } else if (current_mode == GRAPHICS) {
        vga_draw_glyph(0, 0, WHITE, 'A');
    }
} 
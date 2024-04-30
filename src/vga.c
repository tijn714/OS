#include "vga.h"
#include <stdarg.h>

static enum color current_fg = WHITE; // Standaard kleur
static enum color current_bg = BLACK; // Standaard achtergrondkleur

void vga_entry(unsigned char c, enum color fg, enum color bg, int x, int y) {
    u16 *vga = (u16 *)0xB8000;
    vga[y * SCREEN_WIDTH + x] = (bg << 12) | (fg << 8) | c;
}

void vga_init() {
    // VGA Tekstmodus instellen
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, (inportb(0x3D5) & 0xC0) | 14);
    outportb(0x3D4, 0x0B);
    outportb(0x3D5, 0);
}

void set_color(enum color fg, enum color bg) {
    current_fg = fg;
    current_bg = bg;
    u8 color = fg | bg << 4;
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, color);
}

void reset_color() {
    set_color(WHITE, BLACK); // Standaard kleuren herstellen
}

void clear_screen() { // Scherm leegmaken
    u16 *vga = (u16 *)0xB8000;

    for (size_t i = 0; i < SCREEN_SIZE; i++) {
        vga[i] = 0x0720;
    }
}

void kputchar(char c, enum color fg, enum color bg) {
    static int x = 0;
    static int y = 0;

    // als een \n wordt gevonden, ga naar de volgende regel
    if (c == '\n') {
        x = 0;
        y++;
    } 

    // als een \r wordt gevonden, ga terug naar het begin van de regel
    else if (c == '\r') {
        x = 0;
    }

    // als een \t wordt gevonden, voeg 4 spaties toe
    else if (c == '\t') {
        x += 4;
    }

    // als een \b wordt gevonden, verwijder een karakter
    else if (c == '\b') {
        if (x > 0) {
            x--;
            vga_entry(' ', fg, bg, x, y);
        }
    }
    
    else {
        vga_entry(c, fg, bg, x, y);
        x++;
    }

    if (x >= SCREEN_WIDTH) {
        x = 0;
        y++;
    }

    if (y >= SCREEN_HEIGHT) {
        y = 0;
    }
}

void kprint(const char *str, ...) {
    va_list args;
    va_start(args, str);

    while (*str != '\0') {
        if (*str == '%') {
            str++;
            if (*str == 'd') {
                int d = va_arg(args, int);
                char buf[32];
                itoa(d, buf, 10);
                for (int i = 0; buf[i] != '\0'; i++) {
                    kputchar(buf[i], current_fg, current_bg);
                }
            } else if (*str == 'x') {
                int x = va_arg(args, int);
                char buf[32];
                itoa(x, buf, 16);
                for (int i = 0; buf[i] != '\0'; i++) {
                    kputchar(buf[i], current_fg, current_bg);
                }
            } else if (*str == 's') {
                char *s = va_arg(args, char *);
                while (*s != '\0') {
                    kputchar(*s, current_fg, current_bg);
                    s++;
                }
            }
        } else {
            kputchar(*str, current_fg, current_bg);
        }
        str++;
    }

    va_end(args);
}

void kserial(enum color fg, enum color bg, const char *str, ...) {
    va_list args;
    va_start(args, str);

    while (*str != '\0') {
        if (*str == '%') {
            str++;
            if (*str == 'd') {
                int d = va_arg(args, int);
                char buf[32];
                itoa(d, buf, 10);
                for (int i = 0; buf[i] != '\0'; i++) {
                    kputchar(buf[i], fg, bg);
                }
            } else if (*str == 'x') {
                int x = va_arg(args, int);
                char buf[32];
                itoa(x, buf, 16);
                for (int i = 0; buf[i] != '\0'; i++) {
                    kputchar(buf[i], fg, bg);
                }
            } else if (*str == 's') {
                char *s = va_arg(args, char *);
                while (*s != '\0') {
                    kputchar(*s, fg, bg);
                    s++;
                }
            }
        } else {
            kputchar(*str, fg, bg);
        }
        str++;
    }

    va_end(args);
}

#include "vga.h"
#include "system.h"
#include <stdarg.h>

// Globale variabelen voor de huidige tekstkleuren
enum color current_fg = WHITE;
enum color current_bg = BLACK;
size_t vga_index = 0;
size_t vga_row = 0;
size_t vga_column = 0;
volatile uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;

void clear_screen() {
    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            vga_buffer[y * SCREEN_WIDTH + x] = ' ' | (current_bg << 12) | (current_fg << 8);
        }
    }
    vga_index = 0;
    vga_row = 0;
    vga_column = 0;
}

void vga_entry(unsigned char c, enum color fg, enum color bg, int x, int y) {
    vga_buffer[y * SCREEN_WIDTH + x] = c | (bg << 12) | (fg << 8);
}

void vga_init() {
    clear_screen();
}

void set_color(enum color fg, enum color bg) {
    current_fg = fg;
    current_bg = bg;
}

void reset_color() {
    current_fg = WHITE;
    current_bg = BLACK;
}

void kputchar(char c, enum color fg, enum color bg) {
    if (c == '\n') {
        vga_column = 0;
        if (++vga_row == SCREEN_HEIGHT) {
            vga_row = 0;
        }
    } else if (c == '\t') {
        for (int i = 0; i < 4; i++) {
            vga_entry(' ', fg, bg, vga_column, vga_row);
            if (++vga_column == SCREEN_WIDTH) {
                vga_column = 0;
                if (++vga_row == SCREEN_HEIGHT) {
                    vga_row = 0;
                }
            }
        }
    } else if (c == '\b') {
        if (vga_column > 0) {
            vga_entry(' ', fg, bg, vga_column, vga_row);
            vga_column--;
        } else if (vga_row > 0) {
            vga_column = SCREEN_WIDTH - 1;
            vga_row--;
            vga_entry(' ', fg, bg, vga_column, vga_row);
        }
    } else if (c == '\r') {
        vga_column = 0;
    } else {
        vga_entry(c, fg, bg, vga_column, vga_row);
        if (++vga_column == SCREEN_WIDTH) {
            vga_column = 0;
            if (++vga_row == SCREEN_HEIGHT) {
                vga_row = 0;
            }
        }
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
            }  else if (*str == 'c') {
                char c = va_arg(args, int);
                kputchar(c, current_fg, current_bg);
            } else if (*str == '%') {
                kputchar('%', current_fg, current_bg);
            } else {
                kputchar('%', current_fg, current_bg);
                kputchar(*str, current_fg, current_bg);
            }
        } 
        else {
            kputchar(*str, current_fg, current_bg);
        }
        str++;
    }

    va_end(args);
}


void kserial(enum color fg, enum color bg, const char *str, ...) {

    kprint("\n [+] Serial output not implemented yet.\n");
}
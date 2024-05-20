#include "vga.h"
#include "memory.h"
#include "io_ports.h"
#include <stdarg.h>

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
    set_cursor(0, 0);
}

void set_cursor(int x, int y) {
    uint16_t pos = y * SCREEN_WIDTH + x;
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (uint8_t) (pos & 0xFF));
    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void vga_entry(unsigned char c, enum color fg, enum color bg, int x, int y) {
    vga_buffer[y * SCREEN_WIDTH + x] = c | (bg << 12) | (fg << 8);
}

void vga_init() {

    set_color(WHITE, BLACK);
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

void disable_cursor() {
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, 0x20);
}

void enable_cursor() {
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, 0x0D);
    outportb(0x3D4, 0x0B);
    outportb(0x3D5, 0x0E);
}

void kputchar(char c, enum color fg, enum color bg) {
    if (c == '\n') {
        vga_column = 0;
        if (++vga_row == SCREEN_HEIGHT) {
            clear_screen();
        }
    } else if (c == '\t') {
        for (int i = 0; i < 4; i++) {
            vga_entry(' ', fg, bg, vga_column, vga_row);
            if (++vga_column == SCREEN_WIDTH) {
                vga_column = 0;
                if (++vga_row == SCREEN_HEIGHT) {
                    clear_screen();
                }
            }
        }
    } else if (c == '\b') {
        if (vga_column > 0) {
            vga_column--;
        } else if (vga_row > 0) {
            vga_row--;
            vga_column = SCREEN_WIDTH - 1;
        }
        vga_entry(' ', fg, bg, vga_column, vga_row);
    } else if (c == '\r') {
        vga_column = 0;
    } else {
        if (vga_column == SCREEN_WIDTH) {
            vga_column = 0;
            if (++vga_row == SCREEN_HEIGHT) {
                clear_screen();
            }
        }
        vga_entry(c, fg, bg, vga_column, vga_row);
        if (++vga_column == SCREEN_WIDTH) {
            vga_column = 0;
            if (vga_row == SCREEN_HEIGHT) {
                clear_screen();
            }
        }
    }

    set_cursor(vga_column, vga_row);
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
            }  else if (*str == 'c') {
                char c = va_arg(args, int);
                kputchar(c, fg, bg);
            } else if (*str == '%') {
                kputchar('%', fg, bg);
            } else {
                kputchar('%', fg, bg);
                kputchar(*str, fg, bg);
            }
        } 
        else {
            kputchar(*str, fg, bg);
        }
        str++;
    }

    va_end(args);
}

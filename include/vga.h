#ifndef VGA_H
#define VGA_H

#include "types.h"

enum color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};

// Schermgrootte
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)


#define VGA_MEMORY 0xB8000

// Functie declaraties
void clear_screen();

void vga_entry(unsigned char c, enum color fg, enum color bg, int x, int y);
void vga_init();
void set_color(enum color fg, enum color bg);
void reset_color();

void disable_cursor();

void kputchar(char c, enum color fg, enum color bg);
void kprint(const char *str, ...);
void kserial(enum color fg, enum color bg, const char *str, ...);

#endif /* VGA_H */

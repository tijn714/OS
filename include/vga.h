#ifndef VGA_H
#define VGA_H

#include "types.h"

enum vga_modes {
    TEXT,
    GRAPHICS
};

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

#define MAX_SCREEN_WIDTH 320
#define MAX_SCREEN_HEIGHT 200
#define MAX_SCREEN_SIZE (MAX_SCREEN_WIDTH * MAX_SCREEN_HEIGHT)


extern enum vga_modes current_mode;
extern enum color current_bg;
extern enum color current_fg;
extern size_t vga_index;
extern size_t vga_row;
extern size_t vga_column;


#define VGA_MEMORY 0xB8000
#define VGA_VIDEO_MEMORY 0xA0000

#define VGA_MISC_READ  0x3CC
#define VGA_MISC_WRITE 0x3C2

// Sequencer Registers
#define VGA_SEQ_INDEX  0x3C4
#define VGA_SEQ_DATA   0x3C5

// Graphics Controller Registers
#define VGA_GC_INDEX  0x3CE
#define VGA_GC_DATA   0x3CF

// Attribute Controller Registers
#define VGA_AC_INDEX  0x3C0
#define VGA_AC_READ   0x3C1
#define VGA_AC_WRITE  0x3C0

// CRT Controller Registers
#define VGA_CRTC_INDEX  0x3D4
#define VGA_CRTC_DATA   0x3D5

// VGA kleur registers
#define VGA_DAC_READ_INDEX   0x3C7
#define VGA_DAC_WRITE_INDEX  0x3C8
#define VGA_DAC_DATA   0x3C9

// Input Status Register
#define VGA_INSTAT_READ   0x3DA

// Functie declaraties
void clear_screen();

void vga_entry(unsigned char c, enum color fg, enum color bg, int x, int y);
void vga_init();
void set_color(enum color fg, enum color bg);
void reset_color();

void disable_cursor();
void enable_cursor();
void set_cursor(int x, int y);

void kputchar(char c, enum color fg, enum color bg);
void kprint(const char *str, ...);
void kserial(enum color fg, enum color bg, const char *str, ...);

void vga_video_init();
void vga_video_exit();
void vga_video_clear(enum color color);
void vga_video_draw_pixel(int x, int y, enum color color);
void vga_draw_glyph(int x, int y, enum color color, char c);

#endif /* VGA_H */
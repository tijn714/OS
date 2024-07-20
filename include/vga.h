#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_ADDRESS 0xA0000
#define VGA_MAX_WIDTH 320
#define VGA_MAX_HEIGHT 200
#define VGA_MAX (VGA_MAX_WIDTH * VGA_MAX_HEIGHT)

enum color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

typedef enum color color;

#define VGA_TEXT_ADDRESS       0xB8000
#define VGA_TEXT_TOTAL_ITEMS   2200

#define VGA_TEXT_WIDTH    80
#define VGA_TEXT_HEIGHT   24

// Miscellaneous Output
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

// VGA Color Palette Registers
#define VGA_DAC_READ_INDEX   0x3C7
#define VGA_DAC_WRITE_INDEX  0x3C8
#define VGA_DAC_DATA   0x3C9

// General Control and Status Registers
#define VGA_INSTAT_READ   0x3DA

uint16_t vga_text_item_entry(uint8_t ch, color fore_color, color back_color);
void vga_text_set_cursor_pos(uint8_t x, uint8_t y);
void vga_disable_cursor();

void vga_video_init();
void vga_video_exit();
void vga_video_clear_color(uint8_t color);
void vga_video_putpixel(uint16_t x, uint16_t y, uint8_t color);
void vga_video_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);

void draw_glyph(uint16_t x, uint16_t y, uint8_t color, char ch);
void kprint(const char *str, ...);


#endif
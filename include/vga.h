#ifndef VGA_H
#define VGA_H

#include "types.h"
#include "io.h"
#include "mem.h"

// VGA Colors
enum vga_color {
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

#define COPYRIGHT_ICON 0xA9
#define AT_ICON 0x40

// Define VGA registers
#define VGA_MISC_PORT           0x3C2
#define VGA_SEQ_PORT            0x3C4
#define VGA_SEQ_DATA_PORT       0x3C5
#define VGA_GC_PORT             0x3CE
#define VGA_GC_DATA_PORT        0x3CF
#define VGA_CRTC_PORT           0x3D4
#define VGA_CRTC_DATA_PORT      0x3D5
#define VGA_INSTAT_READ_PORT    0x3DA

// VGA Registers
#define VGA_MISC_REG            0x3CC
#define VGA_SEQ_REG             0x3C4
#define VGA_GC_REG              0x3CE
#define VGA_CRTC_REG            0x3D4

uint16_t vga_entry(unsigned char uc, uint8_t color);
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

// VGA Text mode (80x25)
void vga_init(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void kprint(const char* data, ...);
void clear_screen(void);

// VGA Graphics mode (320x200) TODO
//
// NEED TO IMPLEMENT LATER
//
/////////////////////////////////////


void loading_bar(const char* data, int current, int total);
void print_ok(const char* data);
void print_fail(const char* data);
void print_info(const char* data);

#endif // VGA_H

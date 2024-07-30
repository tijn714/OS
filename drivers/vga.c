#include "vga.h"
#include "memory.h"
#include "font.h"
#include "io_ports.h"
#include <stdarg.h>

enum vga_modes current_mode = TEXT;
enum color current_fg = WHITE;
enum color current_bg = BLACK;
size_t vga_index = 0;
size_t vga_row = 0;
size_t vga_column = 0;
volatile uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;
volatile uint8_t* vga_graphics = (uint8_t*)VGA_VIDEO_MEMORY;

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
    current_mode = TEXT;
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

void set_miscellaneous_registers() {
    outportb(VGA_MISC_WRITE, 0x63);
}

void set_sequencer_registers() {
    outportb(VGA_SEQ_INDEX, 0);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x03);  // sequencer reset

    outportb(VGA_SEQ_INDEX, 1);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x01);  // clocking mode

    outportb(VGA_SEQ_INDEX, 2);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x0F);  // plane/map mask

    outportb(VGA_SEQ_INDEX, 3);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x00);  // character font

    outportb(VGA_SEQ_INDEX, 4);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x0E);  // memory mode register
}

void set_graphics_controller_registers() {
    outportb(VGA_GC_INDEX, 0);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // set/reset register

    outportb(VGA_GC_INDEX, 1);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // enable set/reset register

    outportb(VGA_GC_INDEX, 2);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // color compare register

    outportb(VGA_GC_INDEX, 3);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // data rotate register

    outportb(VGA_GC_INDEX, 4);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // read plane select register

    outportb(VGA_GC_INDEX, 5);    // gc index
    outportb(VGA_GC_DATA, 0x40);  // graphics mode register

    outportb(VGA_GC_INDEX, 6);    // gc index
    outportb(VGA_GC_DATA, 0x05);  // miscellaneous register

    outportb(VGA_GC_INDEX, 7);    // gc index
    outportb(VGA_GC_DATA, 0x0F);  // color dont care register

    outportb(VGA_GC_INDEX, 8);    // gc index
    outportb(VGA_GC_DATA, 0xFF);  // bit mask register
}

void set_attribute_controller_registers() {

    uint8_t d;
    uint8_t ac_data[21] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
        0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00, 0x00
    };

    for (uint8_t index = 0; index < 21; index++) {
        outportb(VGA_AC_INDEX, index);
        outportb(VGA_AC_WRITE, ac_data[index]);
    }
    d = inportb(VGA_INSTAT_READ);
    outportb(VGA_AC_INDEX, d | 0x20);
}

void set_crt_controller_registers() {
    uint8_t crtc_data[25] = {
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 0xFF
    };

    for (uint8_t index = 0; index < 25; index++) {
        outportb(VGA_CRTC_INDEX, index);
        outportb(VGA_CRTC_DATA, crtc_data[index]);
    }
}

void vga_video_init() {
    set_miscellaneous_registers();
    set_sequencer_registers();
    set_graphics_controller_registers();
    set_attribute_controller_registers();
    set_crt_controller_registers();
    // set the mode
    current_mode = GRAPHICS;
}


void vga_video_exit() {
    uint8_t i;
    // clear miscellaneous registers
    outportb(VGA_MISC_WRITE, 0x00);
    // clear sequencer registers
    for (i = 0; i < 5; i++) {
        outportb(VGA_GC_INDEX, i);
        outportb(VGA_GC_DATA, 0x00);
    }
    // clear crt controller registers
    for (i = 0; i < 25; i++) {
        outportb(VGA_CRTC_INDEX, i);
        outportb(VGA_CRTC_DATA, 0x00);
    }
    // clear graphics controller registers
    for (i = 0; i < 9; i++) {
        outportb(VGA_GC_INDEX, i);
        outportb(VGA_GC_DATA, 0x00);
    }
    // clear ac registers
    for (i = 0; i < 21; i++) {
        outportb(VGA_AC_INDEX, i);
        outportb(VGA_AC_WRITE, 0x00);
    }
    outportb(VGA_AC_INDEX, 0x00);

    // set the mode
    current_mode = TEXT;
}

void vga_video_clear(enum color color) {
    for (uint8_t index = 0; index < MAX_SCREEN_SIZE; index++) {
        vga_graphics[index] = color;
    }
}

void vga_video_draw_pixel(int x, int y, enum color color) {
    uint32_t index = 0;
    index = MAX_SCREEN_WIDTH * y + x;
    if (index < MAX_SCREEN_SIZE) {
        vga_graphics[index] = color;
    }
}


// function that load the font glyph from the static const uint8_t font[128][FONT_WIDTH] array
void vga_draw_glyph(int x, int y, enum color color, char c) {
    uint8_t glyph[FONT_WIDTH];
    for (int i = 0; i < FONT_WIDTH; i++) {
        glyph[i] = font[(int)c][i];
    }
    for (int i = 0; i < FONT_WIDTH; i++) {
        for (int j = 0; j < FONT_HEIGHT; j++) {
            if ((glyph[i] >> j) & 1) {
                vga_video_draw_pixel(x + j, y + i, color);
            }
        }
    }
}
#include "vga.h"
#include "memory.h"
#include "io_ports.h"
#include "font.h"
#include <stdarg.h>

static uint8_t* g_vga_buffer;

uint16_t vga_text_item_entry(uint8_t ch, color fore_color, color back_color) {
    uint16_t ax = 0;
    uint8_t ah = 0, al = 0;

    ah = back_color;
    ah <<= 4;
    ah |= fore_color;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;

    return ax;
}

void vga_text_set_cursor_pos(uint8_t x, uint8_t y) {
    // The screen is 80 characters wide...
    uint16_t cursorLocation = y * VGA_TEXT_WIDTH + x;
    outportb(0x3D4, 14);
    outportb(0x3D5, cursorLocation >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, cursorLocation);
}

void vga_disable_cursor() {
    outportb(0x3D4, 10);
    outportb(0x3D5, 32);
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
    uint8_t ac_data[21] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                         0x41, 0x00, 0x0F, 0x00, 0x00};

    for (uint8_t index = 0; index < 21; index++) {
        outportb(VGA_AC_INDEX, index);
        outportb(VGA_AC_WRITE, ac_data[index]);
    }
    d = inportb(VGA_INSTAT_READ);
    outportb(VGA_AC_INDEX, d | 0x20);
}

void set_crt_controller_registers() {
    uint8_t crtc_data[25] = {0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
                            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 0xFF};
    
    for (uint8_t index = 0; index < 25; index++) {
        outportb(VGA_CRTC_INDEX, index);
        outportb(VGA_CRTC_DATA, crtc_data[index]);
    }
}


void vga_video_init() {
    set_miscellaneous_registers();
    set_sequencer_registers();
    set_crt_controller_registers();
    set_graphics_controller_registers();
    set_attribute_controller_registers();

    g_vga_buffer = (uint8_t*)VGA_ADDRESS;

    vga_video_clear_color(BLACK);
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

}

void vga_video_clear_color(uint8_t color) {
    for (uint32_t index = 0; index < VGA_MAX; index++)
        g_vga_buffer[index] = color;
}

void vga_video_putpixel(uint16_t x, uint16_t y, uint8_t color) {
    uint32_t index = 0;
    index = 320 * y + x;
    if (index < VGA_MAX)
        g_vga_buffer[index] = color;
}

void vga_video_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color) {
    if (y1 == y2) {
        for (uint16_t i = x1; i <= x2; i++)
            vga_video_putpixel(i, y1, color);
        return;
    }

    if (x1 == x2) {
        for (uint16_t i = y1; i <= y2; i++) {
            vga_video_putpixel(x1, i, color);
        }
        return;
    }
}

uint8_t* font_get_glyph(char ch) {
    return font + (ch * 16);
}

void draw_glyph(uint16_t x, uint16_t y, uint8_t color, char ch) {
    uint8_t* glyph = font_get_glyph(ch);
    uint16_t i, j;
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 8; j++) {
            if ((glyph[i] >> j) & 1) {
                vga_video_putpixel(x + j, y + i, color);
            }
        }
    }
}
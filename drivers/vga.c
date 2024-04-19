#include "vga.h"
#include <stdarg.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void vga_init(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(WHITE, BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}


void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      terminal_row = 0;
    }
  } else if(c == '\t') {
    terminal_column += 4;
    if (terminal_column >= VGA_WIDTH) {
      terminal_column = 0;
      if (++terminal_row == VGA_HEIGHT) {
        terminal_row = 0;
      }
    }
  } else if(c == '\r') {
    terminal_column = 0;
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      if (++terminal_row == VGA_HEIGHT) {
        terminal_row = 0;
      }
    }
  }
}


void kprint(const char* data, ...) {
  va_list args;
  va_start(args, data);
  for (size_t i = 0; data[i] != '\0'; i++) {
    if (data[i] == '%') {
      i++;
      if (data[i] == 'd') {
        int d = va_arg(args, int);
        char buf[32];
        itoa(d, buf, 10);
        for (size_t j = 0; buf[j] != '\0'; j++) {
          terminal_putchar(buf[j]);
        }
      } else if (data[i] == 'x') {
        int x = va_arg(args, int);
        char buf[32];
        itoa(x, buf, 16);
        for (size_t j = 0; buf[j] != '\0'; j++) {
          terminal_putchar(buf[j]);
        }
      } else if (data[i] == 's') {
        char* s = va_arg(args, char*);
        for (size_t j = 0; s[j] != '\0'; j++) {
          terminal_putchar(s[j]);
        }
      } else if (data[i] == 'c') {
        char c = va_arg(args, int);
        terminal_putchar(c);
      }
      else if (data[i] == '%') {
        terminal_putchar('%');
      }
    } else {
      terminal_putchar(data[i]);
    }
  }
  va_end(args);
}

void clear_screen(void) {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
  terminal_row = 0;
  terminal_column = 0;
}

void print_ok(const char* data) {
  terminal_setcolor(vga_entry_color(GREEN, BLACK));
  kprint(" [OK] %s\n", data);
  terminal_setcolor(vga_entry_color(WHITE, BLACK));
}

void print_fail(const char* data) {
  terminal_setcolor(vga_entry_color(RED, BLACK));
  kprint(" [FAILED] %s\n", data);
  terminal_setcolor(vga_entry_color(WHITE, BLACK));
}

void print_info(const char* data) {
  terminal_setcolor(vga_entry_color(BLUE, BLACK));
  kprint(" [INFO] %s\n", data);
  terminal_setcolor(vga_entry_color(WHITE, BLACK));
}


void loading_bar(const char* data, int current, int total) {
  current++; // make the current value start from 1
  int percentage = (current * 100) / total;

  kprint("\r %s: %d of %d (%d%% complete)", data, current, total, percentage);

  if (current == total) {
    kprint("\r %s: %d of %d (%d%% complete)\n", data, current, total, percentage);
  }

}
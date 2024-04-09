#include "types.h"
#include "memory.h"
#include "vga.h"

void vga_init(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			VGA_MEMORY[index] = vga_entry(' ', terminal_color);
		}
	}
}

void vga_setcolor(uint8_t color)
{
	terminal_color = color;
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	VGA_MEMORY[index] = vga_entry(c, color);
}

void vga_putchar(char c)
{
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
	} else {
		vga_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
	}
}

void vga_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		vga_putchar(data[i]);
}

void vga_print(const char* data, ...)
{
	va_list args;
	va_start(args, data);
	size_t len = strlen(data);
	for (size_t i = 0; i < len; i++) {
		if (data[i] == '%') {
			i++;
			switch (data[i]) {
				case 's': {
					const char* str = va_arg(args, const char*);
					vga_write(str, strlen(str));
					break;
				}
				case 'c': {
					char c = va_arg(args, int);
					vga_putchar(c);
					break;
				}
				case 'd': {
					int num = va_arg(args, int);
					char str[32];
					itoa(num, str, 10);
					vga_write(str, strlen(str));
					break;
				}
				case 'x': {
					int num = va_arg(args, int);
					char str[32];
					itoa(num, str, 16);
					vga_write(str, strlen(str));
					break;
				}
				case 'b': {
					int num = va_arg(args, int);
					char str[32];
					itoa(num, str, 2);
					vga_write(str, strlen(str));
					break;
				}
				case '%': {
					vga_putchar('%');
					break;
				}
			}
		} else {
			vga_putchar(data[i]);
		}
	}
	va_end(args);
}

void vga_clear(void)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			VGA_MEMORY[index] = vga_entry(' ', terminal_color);
		}
	}
	terminal_row = 0;
	terminal_column = 0;
}

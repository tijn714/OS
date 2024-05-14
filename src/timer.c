#include "system.h"
#include "irq.h"
#include "vga.h"

void timer_phase(int32_t hz)
{
	int32_t div = 1193180 / hz;

	outb(0x43, 0x36);
	outb(0x40, div & 0xFF);
	outb(0x40, div >> 8);
}

int32_t timer_ticks = 0;
unsigned long ticker = 0;

void timer_handler(struct regs* r)
{
	++timer_ticks;

	if (timer_ticks % 18 == 0)
	{
		ticker++;
	}
}

void timer_init()
{
	irq_install_handler(0, timer_handler);
}

void sleep(int32_t ticks)
{
	kprint("\n");
	unsigned long eticks;
	eticks = timer_ticks + ticks;

	while (timer_ticks < eticks) {
		kprint("\r Ticks remaining: %d", eticks - timer_ticks);
	}
	kprint("\n");
}

void print_ticks()
{
	for (;;) {	
		kprint("\rTicks: %d", ticker);
	}
}
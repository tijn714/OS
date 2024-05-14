#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"

extern void timer_init();
extern int32_t timer_ticks;
extern void sleep(int32_t ticks);
extern void print_ticks();
#endif /* __TIMER_H__ */
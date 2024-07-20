#ifndef CPU_H
#define CPU_H

#include "types.h"

void __cpuid(uint32_t type, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

#endif
#include "cpu.h"
#include "io_ports.h"

void __cpuid(uint32_t type, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    __asm__ __volatile__("cpuid"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(type));
}
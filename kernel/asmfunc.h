/* kernel/asmfunc.h */

#ifndef ASMFUNC_H_INCLUDED
#define ASMFUNC_H_INCLUDED

#include <stdint.h>

static inline void _io_hlt(void)
{
    asm volatile ("hlt;");
}

static inline void _io_cli(void)
{
    asm volatile ("cli;");
}

static inline void _io_out8(const uint16_t port, const uint8_t data)
{
    asm volatile ("out %%al, %%dx" : : "a"(data), "d"(port));
}

static inline int32_t _io_load_eflags(void)
{
    int32_t ret;
    asm volatile ("pushf; pop %0;" : "=a"(ret));
    return ret;
}

static inline void _io_store_eflags(const int32_t value)
{
    asm volatile ("push %0; popf;" : : "a"(value));
}

static inline void _lgdt(const uint16_t size, const uint32_t addr)
{
    const uint64_t gdt = (uint64_t) 0 | ((uint64_t) addr) << 16 | (uint64_t) size;
    asm volatile ("lgdt %0;" : : "m"(gdt));
}

static inline void _lidt(const uint16_t size, const uint32_t addr)
{
    const uint64_t idt = (uint64_t) 0 | ((uint64_t) addr) << 16 | (uint64_t) size;
    asm volatile ("lidt %0;" : : "m"(idt));
}

#endif /* ASMFUNC_H_INCLUDED */


/* kernel/dsctbl.h */

#ifndef DSCTBL_H_INCLUDED
#define DSCTBL_H_INCLUDED

#include <stdint.h>

typedef struct {
    uint64_t limit_low: 16;
    uint64_t base_low: 16;
    uint64_t base_mid: 8;
    uint64_t type: 4;
    uint64_t S: 1;
    uint64_t DPL: 2;
    uint64_t P: 1;
    uint64_t limit_high: 4;
    uint64_t AVL: 1;
    uint64_t const_0: 1;
    uint64_t D_B: 1;
    uint64_t G: 1;
    uint64_t base_high: 8;
} __attribute__((packed)) segment_descriptor;

typedef struct {
    uint64_t offset_low: 16;
    uint64_t selector: 16;
    uint64_t : 5;
    uint64_t unused: 3;
    uint64_t type: 3;
    uint64_t D: 1;
    uint64_t const_0: 1;
    uint64_t DPL: 2;
    uint64_t P: 1;
    uint64_t offset_high: 16;
} __attribute__((packed)) gate_descriptor;

#define GDT_NUM_MAX 8192
#define NOT_ACCESSED 0b0000
#define ACCESSED_ALREADY 0b0001
#define READ_ONLY 0b0000
#define READABLE_AND_WRITEABLE 0b0010
#define EXECUTE_ONLY 0b0000
#define EXECUTABLE_AND_READABLE 0b0010
#define EXPAND_UPWARD 0b0000
#define EXPAND_DOWNWARD 0b0100
#define NON_CONFORMING 0b0000
#define CONFORMING 0b0100
#define DATA_SEGMENT 0b0000
#define CODE_SEGMENT 0b1000
#define SYSTEM_SEGMENT 0
#define DATA_OR_CODE_SEGMENT 1
#define SEGMENT_DOESNOT_PRESENT_ON_MEMORY 0
#define SEGMENT_PRESENTS_ON_MEMORY 1
#define SEGMENT_FOR_16BIT 0
#define SEGMENT_FOR_32BIT 1
#define THE_LIMIT_IS_INTERPRETED_AS_IS 0
#define THE_LIMIT_IS_INTERPRETED_AS_MULTIPLIED_BY_4KB_IS 1
#define IDT_NUM_MAX 256
#define GATE_FOR_16BIT 0
#define GATE_FOR_32BIT 1

extern void init_gdtidt(void);
extern void set_segmdesc(segment_descriptor *, uint32_t, const uint32_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, uint8_t);
extern void set_gatedesc(gate_descriptor *, const uint32_t, const uint16_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t);

#endif /* DSCTBL_H_INCLUDED */


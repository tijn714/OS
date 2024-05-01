#include "isr.h"
#include "idt.h"
#include "system.h"

#define NUM_ISRS 48

extern void exception_0(struct Registers*);
extern void exception_1(struct Registers*);
extern void exception_2(struct Registers*);
extern void exception_3(struct Registers*);
extern void exception_4(struct Registers*);
extern void exception_5(struct Registers*);
extern void exception_6(struct Registers*);
extern void exception_7(struct Registers*);
extern void exception_8(struct Registers*);
extern void exception_9(struct Registers*);
extern void exception_10(struct Registers*);
extern void exception_11(struct Registers*);
extern void exception_12(struct Registers*);
extern void exception_13(struct Registers*);
extern void exception_14(struct Registers*);
extern void exception_15(struct Registers*);
extern void exception_16(struct Registers*);
extern void exception_17(struct Registers*);
extern void exception_18(struct Registers*);
extern void exception_19(struct Registers*);
extern void exception_20(struct Registers*);
extern void exception_21(struct Registers*);
extern void exception_22(struct Registers*);
extern void exception_23(struct Registers*);
extern void exception_24(struct Registers*);
extern void exception_25(struct Registers*);
extern void exception_26(struct Registers*);
extern void exception_27(struct Registers*);
extern void exception_28(struct Registers*);
extern void exception_29(struct Registers*);
extern void exception_30(struct Registers*);
extern void exception_31(struct Registers*);
extern void exception_32(struct Registers*);
extern void exception_33(struct Registers*);
extern void exception_34(struct Registers*);
extern void exception_35(struct Registers*);
extern void exception_36(struct Registers*);
extern void exception_37(struct Registers*);
extern void exception_38(struct Registers*);
extern void exception_39(struct Registers*);
extern void exception_40(struct Registers*);
extern void exception_41(struct Registers*);
extern void exception_42(struct Registers*);
extern void exception_43(struct Registers*);
extern void exception_44(struct Registers*);
extern void exception_45(struct Registers*);
extern void exception_46(struct Registers*);
extern void exception_47(struct Registers*);

static void (*stubs[NUM_ISRS])(struct Registers*) = {
    exception_0,
    exception_1,
    exception_2,
    exception_3,
    exception_4,
    exception_5,
    exception_6,
    exception_7,
    exception_8,
    exception_9,
    exception_10,
    exception_11,
    exception_12,
    exception_13,
    exception_14,
    exception_15,
    exception_16,
    exception_17,
    exception_18,
    exception_19,
    exception_20,
    exception_21,
    exception_22,
    exception_23,
    exception_24,
    exception_25,
    exception_26,
    exception_27,
    exception_28,
    exception_29,
    exception_30,
    exception_31,
    exception_32,
    exception_33,
    exception_34,
    exception_35,
    exception_36,
    exception_37,
    exception_38,
    exception_39,
    exception_40,
    exception_41,
    exception_42,
    exception_43,
    exception_44,
    exception_45,
    exception_46,
    exception_47,
};

static const char *exceptions[32] = {
    "Divide by zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "OOB",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unrecognized interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED"
};

static struct {
    size_t index;
    void (*stub)(struct Registers*);
} isrs[NUM_ISRS];

static void (*handlers[NUM_ISRS])(struct Registers*) = { 0 };

void isr_install(size_t i, void (*handler)(struct Registers*)) {
    handlers[i] = handler;
}

// referenced from start.S
void isr_handler(struct Registers *regs) {
    if (handlers[regs->int_no]) {
        handlers[regs->int_no](regs);
    }
}

static void exception_handler(struct Registers *regs) {
    panic(exceptions[regs->int_no]);
}

void isr_init() {
    for (size_t i = 0; i < NUM_ISRS; i++) {
        isrs[i].index = i;
        isrs[i].stub = stubs[i];
        idt_set(isrs[i].index, isrs[i].stub, 0x08, 0x8E);
    }

    for (size_t i = 0; i < 32; i++) {
        isr_install(i, exception_handler);
    }
}
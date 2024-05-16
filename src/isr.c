#include "isr.h"
#include "idt.h"
#include "pic.h"
#include "vga.h"

// For both exceptions and irq interrupt
ISR g_interrupt_handlers[NO_INTERRUPT_HANDLERS];

// for more details, see Intel manual -> Interrupt & Exception Handling
char *exception_messages[32] = {
    "0x00   : Division By Zero",
    "0x01   : Debug",
    "0x02   : Non Maskable Interrupt",
    "0x03   : Breakpoint",
    "0x04   : Overflow",
    "0x05   : BOUND Range Exceeded",
    "0x06   : Invalid Opcode",
    "0x07   : Device Not Available (No Math Coprocessor)",
    "0x08   : Double Fault",
    "0x09   : Coprocessor Segment Overrun",
    "0x10   : Invalid TSS",
    "0x11   : Segment Not Present",
    "0x12   : Stack-Segment Fault",
    "0x13   : General Protection",
    "0x14   : Page Fault",
    "0x15   : Unknown Interrupt (intel reserved)",
    "0x16   : x87 FPU Floating-Point Error (Math Fault)",
    "0x17   : Alignment Check",
    "0x18   : Machine Check",
    "0x19   : SIMD Floating-Point Exception",
    "0x20   : Virtualization Exception",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved",
    "0x00FF : Reserved"
};

/**
 * register given handler to interrupt handlers at given num
 */
void isr_register_interrupt_handler(int num, ISR handler) {
    // kprint("IRQ %d registered\n", num);
    if (num < NO_INTERRUPT_HANDLERS)
        g_interrupt_handlers[num] = handler;
}

void isr_end_interrupt(int num) {
    pic8259_eoi(num);
}

void isr_irq_handler(REGISTERS *reg) {
    if (g_interrupt_handlers[reg->int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg->int_no];
        handler(reg);
    }
    pic8259_eoi(reg->int_no);
}

static void print_registers(REGISTERS *reg) {
    kprint(" Systeem registers: \n");
    kprint(" eax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
    kprint(" edi=0x%x, esi=0x%x, ebp=0x%x, esp=0x%x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
    kprint(" eip=0x%x, cs=0x%x, ss=0x%x, eflags=0x%x, useresp=0x%x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);
}

void isr_exception_handler(REGISTERS reg) {
    if (reg.int_no < 32) {

        set_color(WHITE, BLUE);
        clear_screen();
        kprint("\n\n");
        kprint(" Er is een fout opgetreden\n\n");
        kprint(" %s\n", exception_messages[reg.int_no]);
        print_registers(&reg);

        kprint("\n\n Als je dit scherm ziet, is er iets misgegaan, herstart de computer en probeer \n");
        kprint(" het opnieuw.\n");

        disable_cursor();
        
        for (;;)
            ;
    }
    if (g_interrupt_handlers[reg.int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg.int_no];
        handler(&reg);
    }
}

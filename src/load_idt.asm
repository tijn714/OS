section .text
    global idt_load

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
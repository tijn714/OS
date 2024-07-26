; IDT
global load_idt
load_idt:
	mov eax, [esp + 4]  ; get idt pointer
	lidt [eax]          ; load idt
	ret

[BITS 32]

global _start
_start:
	mov esp, _system_stack
	jmp _stublet

align 4
mboot:
	;; MULTIBOOT constants
	MULTIBOOT_PAGE_ALIGN equ 1<<0
	MULTIBOOT_MEM_INFO equ 1<<1
	MULTIBOOT_AOUT_KLUDGE equ 1<<16
	MULTIBOOT_MAGIC_NUM equ 0x1BADB002
	MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEM_INFO | MULTIBOOT_AOUT_KLUDGE
	MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC_NUM + MULTIBOOT_HEADER_FLAGS)

	extern code, bss, end

	;; GRUB MULTIBOOT Header & Boot Signature
	dd MULTIBOOT_MAGIC_NUM
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM

	dd mboot
	dd code
	dd bss
	dd end
	dd _start

;; KERNEL ENTRY POINT
_stublet:
	extern kmain
	call kmain
	jmp $

section .bss
	resb 8192	; 8MB Memory Reserved
_system_stack:
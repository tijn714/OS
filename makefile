CC=i686-elf-gcc
ASM=nasm

GFLAGS=
CCFLAGS= -std=gnu99 -ffreestanding -Wall -Wextra -I include -fno-pie -fno-stack-protector
CCFLAGS+= -fno-builtin-function -fno-builtin
ASFLAGS= -f elf32
LDFLAGS= -ffreestanding -nostdlib -T linker.ld

NAME=RodrigoX

BOOT_SRC=$(wildcard boot/*.asm)
BOOT_OBJ=$(BOOT_SRC:.asm=.o)

DRIVERS_C_SRC=$(wildcard drivers/*.c)
DRIVERS_ASM_SRC=$(wildcard drivers/*.asm)
DRIVERS_OBJ=$(DRIVERS_C_SRC:.c=.o) $(DRIVERS_ASM_SRC:.asm=.o)

KERNEL_SRC=$(wildcard kernel/*.c)
KERNEL_OBJ=$(KERNEL_SRC:.c=.o)


all: bootsect drivers kernel 
	$(CC) $(LDFLAGS) -o $(NAME)-kernel.bin $(BOOT_OBJ) $(DRIVERS_OBJ) $(KERNEL_OBJ)
	@make iso
	

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASFLAGS) $< -o $@

bootsect: $(BOOT_OBJ)


drivers: $(DRIVERS_OBJ)

kernel: $(KERNEL_OBJ)

iso:
	mkdir -p isodir/boot/grub
	cp $(NAME)-kernel.bin isodir/boot/$(NAME)-kernel.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(NAME).iso isodir -volid "$(NAME)"


clean:
	rm -f boot/*.o drivers/*.o kernel/*.o
	rm -f $(NAME)-kernel.bin
	rm -f $(NAME).iso
	rm -rf isodir

run:
	qemu-system-i386 -rtc base=localtime -cdrom $(NAME).iso
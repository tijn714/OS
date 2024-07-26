CC=i686-elf-gcc
ASM=nasm

GFLAGS=
CCFLAGS= -std=gnu99 -ffreestanding -Wall -Wextra -I include
ASFLAGS= -f elf32
LDFLAGS= -ffreestanding -nostdlib -T linker.ld

BOOT_SRC=$(wildcard boot/*.asm)
BOOT_OBJ=$(BOOT_SRC:.asm=.o)

DRIVERS_C_SRC=$(wildcard drivers/*.c)
DRIVERS_ASM_SRC=$(wildcard drivers/*.asm)
DRIVERS_OBJ=$(DRIVERS_C_SRC:.c=.o) $(DRIVERS_ASM_SRC:.asm=.o)

KERNEL_SRC=$(wildcard kernel/*.c)
KERNEL_OBJ=$(KERNEL_SRC:.c=.o)


all: bootsect drivers kernel
	$(CC) $(LDFLAGS) -o os.bin $(BOOT_OBJ) $(DRIVERS_OBJ) $(KERNEL_OBJ)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASFLAGS) $< -o $@

bootsect: $(BOOT_OBJ)


drivers: $(DRIVERS_OBJ)

kernel: $(KERNEL_OBJ)

iso: all
	mkdir -p isodir/boot/grub
	cp os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir


clean:
	rm -f boot/*.o drivers/*.o kernel/*.o os.bin os.iso
	rm -rf isodir

run:
	qemu-system-i386 -rtc base=localtime -cdrom os.iso
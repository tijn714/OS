CC=i686-elf-gcc
AS=nasm

BIN=bin
SRC=src

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASFLAGS=-f elf32
LDFLAGS=-T $(SRC)/linker.ld -nostdlib 

TARGET=OS

default: 
	@mkdir -p $(BIN)
	$(AS) $(ASFLAGS) $(SRC)/entry.asm -o $(BIN)/entry.o
	$(CC) -c $(SRC)/vga.c -o $(BIN)/vga.o $(CFLAGS)
	$(CC) -c $(SRC)/main.c -o $(BIN)/kernel.o $(CFLAGS)

	$(CC) $(LDFLAGS) 	$(BIN)/entry.o 			\
						$(BIN)/vga.o 			\
						$(BIN)/kernel.o 		\
						-o $(TARGET).bin

iso: default
	@mkdir -p isodir/boot/grub
	@cp $(TARGET).bin isodir/boot/$(TARGET).bin
	@echo "set timeout=0" > isodir/boot/grub/grub.cfg
	@echo "set default=0" >> isodir/boot/grub/grub.cfg
	@echo "" >> isodir/boot/grub/grub.cfg
	@echo "menuentry \"$(TARGET)\" {" >> isodir/boot/grub/grub.cfg
	@echo "	multiboot /boot/$(TARGET).bin" >> isodir/boot/grub/grub.cfg
	@echo "	boot" >> isodir/boot/grub/grub.cfg
	@echo "}" >> isodir/boot/grub/grub.cfg
	@grub-mkrescue -o $(TARGET).iso isodir

run:
	qemu-system-i386 -kernel $(TARGET).bin

clean:
	rm -rf $(BIN)
	rm -rf isodir
	rm -rf $(TARGET).bin
	rm -rf $(TARGET).iso
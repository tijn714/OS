CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS= -T config/linker.ld -nostdlib
AS=nasm
ASFLAGS=-f elf32
TARGET=OS
BIN=bin

default:
	@mkdir -p $(BIN)
	$(MAKE) bootstrap
	$(MAKE) kernel
	$(MAKE) link
	
	
bootstrap:
	@mkdir -p $(BIN)
	$(AS) $(ASFLAGS) src/bootstrap.asm -o $(BIN)/bootstrap.o
	# build all the other files


link:
	$(CC) -o $(TARGET).bin $(BIN)/*.o $(CFLAGS) $(LDFLAGS)
	
kernel:
	$(CC) -c src/kernel.c -o $(BIN)/kernel.o $(CFLAGS)
	$(CC) -c src/vga.c -o $(BIN)/vga.o $(CFLAGS)
	$(CC) -c src/idt.c -o $(BIN)/idt.o $(CFLAGS)
	$(CC) -c src/gdt.c -o $(BIN)/gdt.o $(CFLAGS)
	$(CC) -c src/isrs.c -o $(BIN)/isrs.o $(CFLAGS)
	$(CC) -c src/irq.c -o $(BIN)/irq.o $(CFLAGS)
	$(CC) -c src/system.c -o $(BIN)/system.o $(CFLAGS)
	$(CC) -c src/timer.c -o $(BIN)/timer.o $(CFLAGS)


iso:
	mkdir -p $(BIN)/isodir/boot/grub
	cp $(TARGET).bin $(BIN)/isodir/boot/$(TARGET).bin
	echo "set timeout=0" > $(BIN)/isodir/boot/grub/grub.cfg
	echo "set default=0" >> $(BIN)/isodir/boot/grub/grub.cfg
	echo "" >> $(BIN)/isodir/boot/grub/grub.cfg
	echo "menuentry \"$(TARGET)\" {" >> $(BIN)/isodir/boot/grub/grub.cfg
	echo "	multiboot /boot/$(TARGET).bin" >> $(BIN)/isodir/boot/grub/grub.cfg
	echo "	boot" >> $(BIN)/isodir/boot/grub/grub.cfg
	echo "}" >> $(BIN)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(TARGET).iso $(BIN)/isodir

clean:
	rm -rf $(BIN)
	rm -rf *.bin
	rm -rf *.iso
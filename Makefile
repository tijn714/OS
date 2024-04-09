AS=nasm
CC=i686-elf-gcc

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASFLAGS=-f elf32
INCLUDE=-I include/

TARGET=OS

BUILD_DIR=build

iso:
	@printf "[== Building $(TARGET).iso  ==]\n"
	@mkdir -p $(BUILD_DIR)
	@$(AS) $(ASFLAGS) asm/entry.asm -o $(BUILD_DIR)/entry.o
	@$(CC) -c src/kernel.c -o $(BUILD_DIR)/kernel.o $(CFLAGS) $(INCLUDE)
	@$(CC) -c src/ports.c -o $(BUILD_DIR)/ports.o $(CFLAGS) $(INCLUDE)
	@$(CC) -c src/memory.c -o $(BUILD_DIR)/memory.o $(CFLAGS) $(INCLUDE)
	@$(CC) -c src/vga.c -o $(BUILD_DIR)/vga.o $(CFLAGS) $(INCLUDE)

	@$(CC) -T linker.ld -o 	$(BUILD_DIR)/$(TARGET).bin 	\
							$(BUILD_DIR)/entry.o 	\
							$(BUILD_DIR)/kernel.o   \
							$(BUILD_DIR)/ports.o 	\
							$(BUILD_DIR)/memory.o 	\
							$(BUILD_DIR)/vga.o 		\
							$(CFLAGS) -nostdlib

	@mkdir -p isodir/boot/grub
	@cp $(BUILD_DIR)/$(TARGET).bin isodir/boot/$(TARGET).bin
	@echo "set timeout=0" > isodir/boot/grub/grub.cfg
	@echo "set default=0" >> isodir/boot/grub/grub.cfg
	@echo "" >> isodir/boot/grub/grub.cfg
	@echo "menuentry \"$(TARGET)\" {" >> isodir/boot/grub/grub.cfg
	@echo "	multiboot /boot/$(TARGET).bin" >> isodir/boot/grub/grub.cfg
	@echo "	boot" >> isodir/boot/grub/grub.cfg
	@echo "}" >> isodir/boot/grub/grub.cfg
	@grub-mkrescue -o $(TARGET).iso isodir
	@printf "[== Done ==]\n"

	

clean:
	rm -rf $(BUILD_DIR)
	rm -rf isodir
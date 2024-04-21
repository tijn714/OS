CC=i686-elf-gcc
AS=nasm
LD=i686-elf-ld

CINCLUDES=-I./include
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra $(CINCLUDES)
ASFLAGS=-f elf32
LDFLAGS=-T config/linker.ld -nostdlib --allow-multiple-definition

BUILD_DIR=bin

TARGET=OS

default:
	@mkdir -p $(BUILD_DIR)
	@printf "[= AS src/entry.asm =]\n"
	@$(AS) $(ASFLAGS) -o $(BUILD_DIR)/entry.o src/entry.asm

	@printf "[= CC drivers/io.c =]\n"
	@$(CC) -c drivers/io.c  -o $(BUILD_DIR)/io.o $(CFLAGS)
	@printf "[= CC drivers/vga.c =]\n"
	@$(CC) -c drivers/vga.c -o $(BUILD_DIR)/vga.o $(CFLAGS)
	@printf "[= CC drivers/mem.c =]\n"
	@$(CC) -c drivers/mem.c -o $(BUILD_DIR)/mem.o $(CFLAGS)
	@printf "[= CC drivers/isr.c =]\n"
	@$(CC) -c drivers/isr.c -o $(BUILD_DIR)/isr.o $(CFLAGS)
	@printf "[= CC drivers/idt.c =]\n"
	@$(CC) -c drivers/idt.c -o $(BUILD_DIR)/idt.o $(CFLAGS)
	@printf "[= CC drivers/gdt.c =]\n"
	@$(CC) -c drivers/gdt.c -o $(BUILD_DIR)/gdt.o $(CFLAGS)
	@printf "[= CC drivers/irq.c =]\n"
	@$(CC) -c drivers/irq.c -o $(BUILD_DIR)/irq.o $(CFLAGS)
	
	@printf "[= CC drivers/keyboard.c =]\n"
	@$(CC) -c drivers/keyboard.c -o $(BUILD_DIR)/keyboard.o $(CFLAGS)


	@printf "[= CC security/aes.c =]\n"
	@$(CC) -c security/aes.c -o $(BUILD_DIR)/aes.o $(CFLAGS)
	@printf "[= CC security/base64.c =]\n"
	@$(CC) -c security/base64.c -o $(BUILD_DIR)/base64.o $(CFLAGS)

	@printf "[= CC src/kernel.c =]\n"
	@$(CC) -c src/kernel.c -o $(BUILD_DIR)/kernel.o $(CFLAGS)


	@printf "[= LD =]\n"
	@$(LD) $(LDFLAGS) 	$(BUILD_DIR)/entry.o 	\
						$(BUILD_DIR)/io.o 		\
						$(BUILD_DIR)/vga.o 		\
						$(BUILD_DIR)/mem.o 		\
						$(BUILD_DIR)/isr.o 		\
						$(BUILD_DIR)/idt.o 		\
						$(BUILD_DIR)/gdt.o 		\
						$(BUILD_DIR)/keyboard.o \
						$(BUILD_DIR)/aes.o 		\
						$(BUILD_DIR)/base64.o 	\
						$(BUILD_DIR)/kernel.o 	\
						-o $(BUILD_DIR)/$(TARGET).bin


iso: 
	@mkdir -p iso/boot/grub
	@cp $(BUILD_DIR)/$(TARGET).bin iso/boot/$(TARGET).bin
	@echo "set timeout=0" > iso/boot/grub/grub.cfg
	@echo "set default=0" >> iso/boot/grub/grub.cfg
	@echo "" >> iso/boot/grub/grub.cfg
	@echo "menuentry \"$(TARGET)\" {" >> iso/boot/grub/grub.cfg
	@echo "	multiboot /boot/$(TARGET).bin" >> iso/boot/grub/grub.cfg
	@echo "	boot" >> iso/boot/grub/grub.cfg
	@echo "}" >> iso/boot/grub/grub.cfg
	@grub-mkrescue -o $(TARGET).iso iso


qemu:
	@qemu-system-x86_64 -kernel bin/$(TARGET).bin -d cpu_reset -monitor stdio

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf iso
	@rm -f $(TARGET).iso

all:
	@make clean
	@make
	@make iso
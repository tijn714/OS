CC=i686-elf-gcc
AS=nasm

CINCLUDES=-I./include
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra $(CINCLUDES)
ASFLAGS=-f elf32
LDFLAGS=-T config/linker.ld -nostdlib

BUILD_DIR=bin

TARGET=OS

default:
	@mkdir -p $(BUILD_DIR)
	@printf "[= AS src/entry.asm =]\n"
	@$(AS) $(ASFLAGS) -o $(BUILD_DIR)/entry.o src/entry.asm
	@printf "[= AS src/load_idt.asm =]\n"
	@$(AS) $(ASFLAGS) -o $(BUILD_DIR)/load_idt.o src/load_idt.asm
	@printf "[= AS src/load_gdt.asm =]\n"
	@$(AS) $(ASFLAGS) -o $(BUILD_DIR)/load_gdt.o src/load_gdt.asm
	@printf "[= AS src/irq.asm =]\n"
	@$(AS) $(ASFLAGS) -o $(BUILD_DIR)/irq.o src/irq.asm
	@printf "[= AS src/exceptions.asm =]\n"
	@$(AS) $(ASFLAGS) -o $(BUILD_DIR)/exceptions.o src/exceptions.asm


	@printf "[= CC drivers/pic.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/pic.c -o $(BUILD_DIR)/pic.o
	@printf "[= CC drivers/io.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/io.c -o $(BUILD_DIR)/io.o
	@printf "[= CC drivers/idt.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/idt.c -o $(BUILD_DIR)/idt.o
	@printf "[= CC drivers/gdt.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/gdt.c -o $(BUILD_DIR)/gdt.o 
	@printf "[= CC drivers/isr.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/isr.c -o $(BUILD_DIR)/isr.o
	@printf "[= CC drivers/mem.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/mem.c -o $(BUILD_DIR)/mem.o
	@printf "[= CC drivers/vga.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/vga.c -o $(BUILD_DIR)/vga.o
	@printf "[= CC drivers/time.c =]\n"
	@$(CC) $(CFLAGS) -c drivers/time.c -o $(BUILD_DIR)/time.o


	@printf "[= CC security/aes.c =]\n"
	@$(CC) $(CFLAGS) -c security/aes.c -o $(BUILD_DIR)/aes.o
	@printf "[= CC security/base64.c =]\n"
	@$(CC) $(CFLAGS) -c security/base64.c -o $(BUILD_DIR)/base64.o


	@printf "[= CC src/kernel.c =]\n"
	@$(CC) $(CFLAGS) -c src/kernel.c -o $(BUILD_DIR)/kernel.o


	@printf "[= LD =]\n"
	@$(CC) $(LDFLAGS)  			\
	$(BUILD_DIR)/entry.o 		\
	$(BUILD_DIR)/load_idt.o 	\
	$(BUILD_DIR)/idt.o 			\
	$(BUILD_DIR)/load_gdt.o 	\
	$(BUILD_DIR)/gdt.o 			\
	$(BUILD_DIR)/irq.o 			\
	$(BUILD_DIR)/exceptions.o 	\
	$(BUILD_DIR)/pic.o 			\
	$(BUILD_DIR)/io.o 			\
	$(BUILD_DIR)/isr.o 			\
	$(BUILD_DIR)/mem.o 			\
	$(BUILD_DIR)/vga.o 			\
	$(BUILD_DIR)/time.o 		\
	$(BUILD_DIR)/aes.o 			\
	$(BUILD_DIR)/base64.o 		\
	$(BUILD_DIR)/kernel.o 		\
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
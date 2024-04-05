
ARCH=i386

AS=$(ARCH)-elf-as
CC=$(ARCH)-elf-gcc
LD=$(ARCH)-elf-ld
ASFLAGS=
TARGET=OS
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc
CINCLUDE = -I include/
BUILD_DIR=bin


check_for_tools:
	@printf "[+] Checking for tools... \n"
	@which $(AS) > /dev/null || (echo "Error: $(AS) not found" && exit 1)
	@which $(CC) > /dev/null || (echo "Error: $(CC) not found" && exit 1)
	@which $(LD) > /dev/null || (echo "Error: $(LD) not found" && exit 1)
	@which grub-file > /dev/null || (echo "Error: grub-file not found" && exit 1)
	@which grub-mkrescue > /dev/null || (echo "Error: grub-mkrescue not found" && exit 1)
	@printf "[=] Done! \n"

kernel:
	# for people who dont have grub installed
	@printf "[+] Building kernel... (without GRUB)\n"
	@$(AS) $(ASFLAGS) src/multiboot.s -o $(BUILD_DIR)/multiboot.o
	@$(CC) -c src/kernel.c -o $(BUILD_DIR)/kernel.o $(CFLAGS) $(CINCLUDE)
	@$(CC) -T link.ld -o $(BUILD_DIR)/$(TARGET).bin $(LDFLAGS) $(BUILD_DIR)/multiboot.o $(BUILD_DIR)/kernel.o
	@printf "[=] Done! \n"
	@printf "Kernel is located at: $(BUILD_DIR)/$(TARGET).bin, you can run it with make run-kernel \n"

iso:
	@make check_for_tools
	@printf "[+] Checking if build dir exists... \n"
	@mkdir -p $(BUILD_DIR)
	@printf "[+] Building multiboot header... \n"
	@$(AS) $(ASFLAGS) src/multiboot.s -o $(BUILD_DIR)/multiboot.o
	@printf "[+] Building kernel... \n"
	@$(CC) -c src/kernel.c -o $(BUILD_DIR)/kernel.o $(CFLAGS) $(CINCLUDE)
	@printf "[+] Linking kernel... \n"
	@$(CC) -T link.ld -o $(BUILD_DIR)/$(TARGET).bin $(LDFLAGS) $(BUILD_DIR)/multiboot.o $(BUILD_DIR)/kernel.o 

	@printf "[+] Checking if kernel is multiboot compliant... \n"
	@grub-file --is-x86-multiboot $(BUILD_DIR)/$(TARGET).bin

	@printf "[+] Creating isodir... \n"
	@mkdir -p isodir/boot/grub
	@printf "[+] Copying kernel to isodir... \n"
	@cp $(BUILD_DIR)/$(TARGET).bin isodir/boot/$(TARGET).bin
	@printf "[+] Creating grub.cfg... \n"
	@echo "set timeout=0" > isodir/boot/grub/grub.cfg
	@echo "set default=0" >> isodir/boot/grub/grub.cfg
	@echo "" >> isodir/boot/grub/grub.cfg
	@echo "menuentry \"$(TARGET)\" {" >> isodir/boot/grub/grub.cfg
	@echo "	multiboot /boot/$(TARGET).bin" >> isodir/boot/grub/grub.cfg
	@echo "	boot" >> isodir/boot/grub/grub.cfg
	@echo "}" >> isodir/boot/grub/grub.cfg
	@printf "[+] Creating iso... \n"
	@grub-mkrescue -o $(BUILD_DIR)/$(TARGET).iso isodir
	@printf "[+] Cleaning up... \n"
	@rm -rf isodir
	@rm -rf $(BUILD_DIR)/*.o
	@rm -rf $(BUILD_DIR)/*.bin
	@printf "[=] Done! \n"
	@printf "ISO is located at: $(BUILD_DIR)/$(TARGET).iso\n"

clean:
	@printf "[+] Cleaning up... \n"
	@rm -rf $(BUILD_DIR)/*.o
	@rm -rf $(BUILD_DIR)/*.bin
	@rm -rf $(BUILD_DIR)/*.iso
	@rm -rf isodir
	@printf "[=] Done! \n"

run:
	@printf "[+] Running $(TARGET).iso in qemu... \n"
	@qemu-system-i386 -cdrom $(BUILD_DIR)/$(TARGET).iso -d cpu_reset -monitor stdio

run-kernel:
	@printf "[+] Running kernel $(TARGET).bin in qemu... \n"
	@qemu-system-i386 -kernel $(BUILD_DIR)/$(TARGET).bin -d cpu_reset -monitor stdio
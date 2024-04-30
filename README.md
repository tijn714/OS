# OS Development Journey

Welcome to my OS development journey! This project is an exploration into the intricate world of operating system development. Below you'll find information about the project, including its current status, requirements, and how to build and run the project. 

### NOTE: STARTED FROM SCRATCH AGAIN!

## Requirements

To build and run the OS, you'll need the following tools installed:

- `i686-elf-gcc`
- `nasm`
- `make`
- ```grub-mkrescue``` (to build the ISO image)
- QEMU

## Building and Running

To build the kernel use

```bash
make
```

To build the ISO image, use the following command:
```bash
make iso
```

To emulate the ISO with QEMU, use:
```bash
qemu-system-x86_64 -cdrom OS.iso
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

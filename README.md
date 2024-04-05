# OS Development Journey

Welcome to my OS development journey! This project is an exploration into the intricate world of operating system development. Below you'll find information about the project, including its current status, requirements, and how to build and run the project.

## ChangeLog

- **v2 (Current)**
  - Added support for CPU operations, VGA text mode, memory management, and time (still in development).
- **v1**
  - Basic kernel that serves as the starting point of the learning progress.

## Requirements

To build and run the OS, you'll need the following tools installed:

- `i386-elf-toolchain` or `i686-elf-toolchain`. You can modify the ```ARCH``` Flag in the makefile. Current is ```i386```.
- GRUB development toolchain (Only for iso disk and bootloader)
- `make`
- QEMU (to run the ISO image)

## Building and Running

To build the ISO image, use the following command:
```bash
make iso
```

To emulate the ISO with QEMU, use:
```bash
make run
```

### You can also build the project without GRUB:

To build the kernel:
```bash
make kernel
```

To emulate the kernel:
```bash
make run-kernel
```


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

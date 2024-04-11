#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"
#include "ports.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

char ReadKeyboard();
void ReadString(char *str);
void ReadInt(int *n);
void ReadHex(int *n);
void press_any_key_to_continue();

#endif // KEYBOARD_H
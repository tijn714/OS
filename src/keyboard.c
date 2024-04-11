#include "keyboard.h"
#include "types.h"
#include "ports.h"

char ReadKeyboard()
{
    while (1)
    {
        if (inportb(0x64) & 0x01)
        {
            switch (inportb(0x60))
            {
            case 0x1E:
                return 'a';
                break;
            case 0x30:
                return 'b';
                break;
            case 0x2E:
                return 'c';
                break;
            case 0x20:
                return 'd';
                break;
            case 0x12:
                return 'e';
                break;
            case 0x21:
                return 'f';
                break;
            case 0x22:
                return 'g';
                break;
            case 0x23:
                return 'h';
                break;
            case 0x17:
                return 'i';
                break;
            case 0x24:
                return 'j';
                break;
            case 0x25:
                return 'k';
                break;
            case 0x26:
                return 'l';
                break;
            case 0x32:
                return 'm';
                break;
            case 0x31:
                return 'n';
                break;
            case 0x18:
                return 'o';
                break;
            case 0x19:
                return 'p';
                break;
            case 0x10:
                return 'q';
                break;
            case 0x13:
                return 'r';
                break;
            case 0x1F:
                return 's';
                break;
            case 0x14:
                return 't';
                break;
            case 0x16:
                return 'u';
                break;
            case 0x2F:
                return 'v';
                break;
            case 0x11:
                return 'w';
                break;
            case 0x2D:
                return 'x';
                break;
            case 0x15:
                return 'y';
                break;
            case 0x2C:
                return 'z';
                break;
            case 0x02:
                return '1';
                break;
            case 0x03:
                return '2';
                break;
            case 0x04:
                return '3';
                break;
            case 0x05:
                return '4';
                break;
            case 0x06:
                return '5';
                break;
            case 0x07:
                return '6';
                break;
            case 0x08:
                return '7';
                break;
            case 0x09:
                return '8';
                break;
            case 0x0A:
                return '9';
                break;
            case 0x0B:
                return '0';
                break;
            case 0x1C:
                return '\n';
                break;
            case 0x39:
                return ' ';
                break;
            case 0x0E:
                return '\b';
                break;
            case 0x1A:
                return '[';
                break;
            case 0x1B:
                return ']';
                break;
            case 0x27:
                return ';';
                break;
            case 0x28:
                return '\'';
                break;
            case 0x29:
                return '`';
                break;
            case 0x33:
                return ',';
                break;
            case 0x34:
                return '.';
                break;
            case 0x35:
                return '/';
                break;
            case 0x2B:
                return '\\';
                break;
            case 0x0C:
                return '-';
                break;
            case 0x0D:
                return '=';
                break;
            case 0x1D:
                return 0;
                break;
            case 0x2A:
                return 0;
                break;
            case 0x36:  
                return 0;
                break;
            default:
                break;
            }
        }
    }
}

void ReadString(char *str)
{
    int i = 0;
    while (1)
    {
        char c = ReadKeyboard();
        if (c == '\n')
        {
            str[i] = '\0';
            break;
        }
        else if (c == '\b')
        {
            if (i > 0)
            {
                i--;
                str[i] = '\0';
            }
        }
        else
        {
            str[i] = c;
            i++;
        }
    }
}

void ReadInt(int *n)
{
    char str[32];
    ReadString(str);
    *n = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        *n = *n * 10 + str[i] - '0';
    }
}

void ReadHex(int *n)
{
    char str[32];
    ReadString(str);
    *n = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        *n = *n * 16;
        if (str[i] >= '0' && str[i] <= '9')
        {
            *n += str[i] - '0';
        }
        else if (str[i] >= 'a' && str[i] <= 'f')
        {
            *n += str[i] - 'a' + 10;
        }
        else if (str[i] >= 'A' && str[i] <= 'F')
        {
            *n += str[i] - 'A' + 10;
        }
    }
}

void press_any_key_to_continue()
{
    char c;
    while (1)
    {
        c = ReadKeyboard();
        if (c != 0)
        {
            break;
        }
    }
}
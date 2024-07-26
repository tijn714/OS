#include "memory.h"
#include "vga.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include "io_ports.h"
#include "timer.h"
#include "keyboard.h"
#include "aes.h"

#include "kernel.h"


void restart() {
    outportb(0x64, 0xFE);
}

// shutdown signal
void shutdown() {
    outportw(0xB004, 0x2000);
}

void __cpuid(uint32_t type, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile("cpuid"
                : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                : "0"(type)); // put the type into eax
}


void print_sysinfo() {
    uint32_t eax, ebx, ecx, edx;
    char brand[49];

    __cpuid(0x80000002, &eax, &ebx, &ecx, &edx);
    memcpy(brand, &eax, 4);
    memcpy(brand + 4, &ebx, 4);
    memcpy(brand + 8, &ecx, 4);
    memcpy(brand + 12, &edx, 4);
    
    __cpuid(0x80000003, &eax, &ebx, &ecx, &edx);
    memcpy(brand + 16, &eax, 4);
    memcpy(brand + 20, &ebx, 4);
    memcpy(brand + 24, &ecx, 4);
    memcpy(brand + 28, &edx, 4);

    __cpuid(0x80000004, &eax, &ebx, &ecx, &edx);
    memcpy(brand + 32, &eax, 4);
    memcpy(brand + 36, &ebx, 4);
    memcpy(brand + 40, &ecx, 4);
    memcpy(brand + 44, &edx, 4);
    brand[48] = '\0';

    kserial(YELLOW, DARK_GRAY, " Processor: %s\n", brand);


}



void kmain(void) {
    vga_init();
    gdt_init();
    idt_init();
    timer_init();
    keyboard_init();

    DATE_TIME dt;
    get_date_time(&dt);

    kserial(GREEN, BLACK, " [info] Systeem opgestart op");


    if (dt.day < 10) {
        kserial(GREEN, BLACK, " 0%d", dt.day);
    } else {
        kserial(GREEN, BLACK, " %d", dt.day);
    }

    if (dt.month == 1) {
        kserial(GREEN, BLACK, " jan");
    } else if (dt.month == 2) {
        kserial(GREEN, BLACK, " feb");
    } else if (dt.month == 3) {
        kserial(GREEN, BLACK, " mrt");
    } else if (dt.month == 4) {
        kserial(GREEN, BLACK, " apr");
    } else if (dt.month == 5) {
        kserial(GREEN, BLACK, " mei");
    } else if (dt.month == 6) {
        kserial(GREEN, BLACK, " jun");
    } else if (dt.month == 7) {
        kserial(GREEN, BLACK, " jul");
    } else if (dt.month == 8) {
        kserial(GREEN, BLACK, " aug");
    } else if (dt.month == 9) {
        kserial(GREEN, BLACK, " sep");
    } else if (dt.month == 10) {
        kserial(GREEN, BLACK, " okt");
    } else if (dt.month == 11) {
        kserial(GREEN, BLACK, " nov");
    } else if (dt.month == 12) {
        kserial(GREEN, BLACK, " dec");
    } else {
        if (dt.month < 10) {
            kserial(GREEN, BLACK, " 0%d", dt.month);
        } else {
            kserial(GREEN, BLACK, " %d", dt.month);
        }
    }

    kserial(GREEN, BLACK, " %d", dt.year);

    kserial(GREEN, BLACK, " om");

    if (dt.hour < 10) {
        kserial(GREEN, BLACK, " 0%d", dt.hour);
    } else {
        kserial(GREEN, BLACK, " %d", dt.hour);
    }

    if (dt.minute < 10) {
        kserial(GREEN, BLACK, ":0%d", dt.minute);
    } else {
        kserial(GREEN, BLACK, ":%d", dt.minute);
    }

    if (dt.second < 10) {
        kserial(GREEN, BLACK, ":0%d", dt.second);
    } else {
        kserial(GREEN, BLACK, ":%d", dt.second);
    }

    kprint("\n\n");

    uint8_t expanded_key[240];

    uint8_t enc_buf[16];
    uint8_t dec_buf[16];

    KeyExpansion(AES_TEST_KEY, expanded_key);

    AES_Encrypt(AES_TEST_PLAINTEXT, enc_buf, expanded_key);
    AES_Decrypt(enc_buf, dec_buf, expanded_key);

    if (!isEqual(AES_TEST_PLAINTEXT, dec_buf, 16)) {
        kserial(RED, BLACK, " AES test mislukt\n");
    } else {
        kserial(GREEN, BLACK, " AES test geslaagd\n");
    }

    kprint("\n\n");
    sleep(1);

    char buffer[256];

    while(1) {
        kprint(" shell@OS~# ");
        kb_getinput(buffer, 256);

        kprint("\n");

        if (strcmp(buffer, "") == 0) {
            continue;
        } 

        if (strcmp(buffer, "clear") == 0) {
            clear_screen();
        } else if (strcmp(buffer, "uptime") == 0) {
            
            int uptime_sec = u_ticks / 100;
            int uptime_min = uptime_sec / 60;
            int uptime_hour = uptime_min / 60;


            kprint(" bedrijfstijd: ");

            if (uptime_hour < 10) {
                kprint("0%d:", uptime_hour);
            } else {
                kprint("%d:", uptime_hour);
            }

            if (uptime_min < 10) {
                kprint("0%d:", uptime_min % 60);
            } else {
                kprint("%d:", uptime_min % 60);
            }

            if (uptime_sec < 10) {
                kprint("0%d\n", uptime_sec % 60);
            } else {
                kprint("%d\n", uptime_sec % 60);
            }
        } else if (strcmp(buffer, "shutdown") == 0) {
            shutdown();
        } else if (strcmp(buffer, "restart") == 0) {
            restart();
        }  else if (strcmp(buffer, "sysinfo") == 0) {
            kserial(YELLOW, DARK_GRAY, " Systeeminformatie\n\n");

            print_sysinfo();

        }
        
        else if (strcmp(buffer, "help") == 0) {
            if (vga_row > 19) {
                clear_screen();
            }

            kprint(" Beschikbare commando's:\n\n");
            kprint(" - clear\n");
            kprint(" - uptime\n");
            kprint(" - restart\n");
            kprint(" - help\n");
            kprint(" - sysinfo\n");
        }
        
        else {
            kserial(RED, BLACK, " commando '%s' niet gevonden.\n", buffer);
        }
        clear_buffer(buffer);
    }
} 

#ifndef FAT_H
#define FAT_H

#include "types.h"
#include "io.h"

#define FAT12 1
#define FAT16 2
#define FAT32 3

// FAT Boot Sector Structure
typedef struct {
    uint8_t  jump_instruction[3];
    uint8_t  oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t  fat_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t  media_type;
    uint16_t fat_size_16;
    uint16_t sectors_per_track;
    uint16_t head_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint8_t  drive_number;
    uint8_t  reserved;
    uint8_t  signature;
    uint32_t volume_serial_number;
    uint8_t  volume_label[11];
    uint8_t  file_system_type[8];
    // Boot code and padding to make the structure 512 bytes
} __attribute__((packed)) fat_boot_sector_t;

// FAT Directory Entry Structure
typedef struct {
    uint8_t  file_name[11];
    uint8_t  attributes;
    uint8_t  reserved;
    uint8_t  creation_time_tenth;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t last_modification_time;
    uint16_t last_modification_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
} __attribute__((packed)) fat_directory_entry_t;

// FAT File System Structure
typedef struct {
    fat_boot_sector_t* boot_sector;
    uint32_t fat_start_sector;
    uint32_t data_start_sector;
    uint32_t root_directory_sector;
    uint32_t bytes_per_cluster;
    uint32_t total_clusters;
    uint8_t fat_type;
} fat_fs_t;

// Function prototypes
fat_fs_t* fat_mount(const char* device_path);
void fat_unmount(fat_fs_t* fs);
int fat_read_file(fat_fs_t* fs, const char* path, void* buffer, size_t size);
int fat_write_file(fat_fs_t* fs, const char* path, void* buffer, size_t size);
void fat_list_directory(fat_fs_t* fs, const char* path);


#endif // FAT_H
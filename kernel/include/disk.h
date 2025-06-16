#ifndef DISK_H
#define DISK_H

#include <stdint.h>

void ata_identify();
void ata_read_sector(uint32_t lba, uint8_t* buffer);
void ata_write_sector(uint32_t lba, const uint8_t* data);

#endif

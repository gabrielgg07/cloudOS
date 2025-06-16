#ifndef NIC_H
#define NIC_H

#include <stdint.h>

typedef struct {
    uint16_t io_base;
    uint8_t irq;
    uint8_t mac[6];
    uint16_t vendor_id;
    uint16_t device_id;
} nic_info_t;


void nic_init();
void print_nic_info();
void pci_scan_for_ne2k();

#endif

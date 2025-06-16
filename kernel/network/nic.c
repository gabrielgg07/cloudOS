#include "nic.h"
#include "../include/terminal.h" 
#include "../include/pci.h" 

nic_info_t global_nic_info;  // Define the global variable

void pci_scan_for_ne2k() {
    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint16_t vendor = pci_config_read_word(bus, slot, 0, 0);
            if (vendor == 0xFFFF) continue; // No device at this slot
            uint16_t device = pci_config_read_word(bus, slot, 0, 2);
            if (vendor == 0x10EC && device == 0x8029) { // Example device ID for NE2000
                terminal_print("NE2000 NIC Found!\n");
                // Initialize global NIC info
                uint32_t bar0 = pci_config_read_word(bus,slot,0,0x10);
                uint8_t irq = pci_config_read_word(bus,slot, 0, 0x3c) & 0xFF;
                global_nic_info.io_base = bar0 & ~0x1; //mask off I/O flag
                global_nic_info.irq = irq;
                global_nic_info.vendor_id = vendor;
                global_nic_info.device_id = device;
                terminal_print("NE2000 I/O base: ");
                terminal_print_int(global_nic_info.io_base);
                terminal_print("\nIRQ: ");
                terminal_print_int(global_nic_info.irq);
                terminal_print("\n");
                return; // Found and initialized, exit
            }
        }
    }
}


void nic_init() {
    // Example values — replace with real detection values
    global_nic_info.io_base = 0x300;   // Or read from PCI BAR
    global_nic_info.irq = 11;          // Or detect via PCI config

    // Example MAC address — replace with real values from NIC
    global_nic_info.mac[0] = 0xDE;
    global_nic_info.mac[1] = 0xAD;
    global_nic_info.mac[2] = 0xBE;
    global_nic_info.mac[3] = 0xEF;
    global_nic_info.mac[4] = 0x00;
    global_nic_info.mac[5] = 0x01;

    global_nic_info.vendor_id = 0x10EC;   // Example: Realtek
    global_nic_info.device_id = 0x8139;   // Example: RTL8139
}

void print_nic_info() {
    // Print NIC information to terminal or log
    // This function can be called after nic_init() to display the info
    // Example implementation:
    terminal_print("NIC Information:\n");
    pci_scan_for_ne2k(); // Scan for NE2000 NIC
}
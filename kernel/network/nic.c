#include "nic.h"
#include "../include/terminal.h"  

nic_info_t global_nic_info;  // Define the global variable


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
    terminal_print("  IO Base: ");
    terminal_print_int(global_nic_info.io_base);
    terminal_print("\n");

    terminal_print("  IRQ Line: ");
    terminal_print_int(global_nic_info.irq);
    terminal_print("\n");

    terminal_print("  MAC Address: ");
    for (int i = 0; i < 6; i++) {
        terminal_print_hex(global_nic_info.mac[i]);
        if (i < 5) terminal_print(":");
    }
    terminal_print("\n");

    terminal_print("  Vendor ID: ");
    terminal_print_int(global_nic_info.vendor_id);
    terminal_print("\n");

    terminal_print("  Device ID: ");
    terminal_print_int(global_nic_info.device_id);
    terminal_print("\n");
}
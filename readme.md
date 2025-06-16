
# ☁️ CloudOS

CloudOS is a hand-built operating system written from scratch in C and assembly. It’s designed to evolve from a basic kernel into a modern, modular OS capable of networking, multitasking, user programs, and eventually cloud-style functionality — all without relying on any existing OS layer.

## 🚀 What CloudOS Can Do (So Far)

✅ Boot from GRUB  
✅ Switch to 32-bit protected mode  
✅ Set up a custom Global Descriptor Table (GDT) and Interrupt Descriptor Table (IDT)  
✅ Implement hardware interrupt handling (IRQ)  
✅ Handle keyboard input via IRQ1  
✅ Display output with a terminal driver (text-mode printing)  
✅ Basic shell/command interface  
✅ Enter and execute in **user mode** (with `iret`)  
✅ Full **syscall** infrastructure using `int 0x80`  
✅ Basic `sysinfo` command using **CPUID** + BIOS scan  
✅ PCI device structure support  
✅ Display NIC info (MAC, IRQ, Vendor ID, etc.)  
✅ ATA disk driver with support for `IDENTIFY` command  
✅ Raw sector **read/write** to disk using ATA PIO  
✅ Shell commands: `sysinfo`, `nicinfo`, `diskinfo`, `diskwrite`, `diskread`

## 🎯 Why I'm Building This

I wanted to:
- Deeply understand **systems-level programming** without relying on an OS
- Build something from the ground up that talks to hardware directly
- Learn how real OSes handle memory, filesystems, syscalls, and multitasking
- Eventually create a custom **cloud-style runtime** that simulates networking and services without a POSIX layer

Also: because writing your own operating system is badass.

## 🛠️ What’s Coming Next

- [ ] Support for **loading and executing user programs** from disk (basic ELF loader or raw binary)
- [ ] Build a **custom filesystem** (or FAT reader) for real disk file storage
- [ ] Basic **multitasking and process scheduling**
- [ ] TCP/IP stack: starting with **Ethernet → ARP → IP → UDP**
- [ ] Networking tools (e.g. UDP echo client/server)
- [ ] Add a `netstat`-like tool to show network status
- [ ] Implement virtual memory + paging
- [ ] Introduce privilege-separated **kernel/user process model**
- [ ] Create a mini **package manager** or "cloud runtime shell"

## 📦 How to Build and Run

1. Clone the repo  
2. Build the project:
   ```bash
   make

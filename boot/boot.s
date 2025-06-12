; boot/boot.s
section .multiboot
align 8
multiboot_header:
    dd 0xe85250d6            ; magic
    dd 0                     ; architecture
    dd header_end - multiboot_header ; length
    dd -(0xe85250d6 + 0 + (header_end - multiboot_header)) ; checksum

    ; Required end tag
    dw 0
    dw 0
    dd 8
header_end:

section .text
global _start
extern kernel_main

_start:
    call kernel_main
    cli
    hlt

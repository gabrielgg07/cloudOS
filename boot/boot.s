section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002              ; magic number
    dd 0x00                    ; flags
    dd -(0x1BADB002 + 0x00)    ; checksum

section .text
global _start
extern kernel_main

_start:

    ; Setup VGA pointer
    mov eax, msg         ; 32-bit load into eax
    mov esi, eax         ; source index (msg)
    mov edi, 0xb8000     ; destination VGA address

.print_loop:
    lodsb                ; load byte from [esi] into al
    test al, al
    je .done
    mov ah, 0x07         ; white-on-black attribute
    stosw                ; store ax at [edi], advance edi
    jmp .print_loop

.done:
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

section .rodata
msg db "Loading kernel...", 0

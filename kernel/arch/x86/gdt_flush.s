[global gdt_flush]
[extern gdt_flush]

gdt_flush:
    mov eax, [esp + 4]     ; Get pointer to gdt_ptr
    lgdt [eax]             ; Load it

    ; Reload segment registers
    mov ax, 0x10           ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush        ; Far jump to reload CS
.flush:
    ret

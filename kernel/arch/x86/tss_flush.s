global tss_flush
tss_flush:
    mov ax, 0x28      ; GDT selector index 5 << 3 = 0x28
    ltr ax
    ret
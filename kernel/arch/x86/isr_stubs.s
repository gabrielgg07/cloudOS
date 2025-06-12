[bits 32]
[global isr_stub_0]
[extern isr_handler]

isr_stub_0:
    cli
    push byte 0
    push byte 0
    call isr_handler
    add esp, 8
    sti
    iret

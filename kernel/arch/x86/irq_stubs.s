[bits 32]
[extern irq_handler]
[extern isr80_handler]

%macro IRQ_STUB 1
  global irq_stub_%1
irq_stub_%1:
  cli
  pusha                   ; <-- Save all general-purpose registers
  push byte 0             ; Push fake error code
  push dword (32 + %1)    ; Push interrupt number
  call irq_handler
  add esp, 8              ; Clean up stack from pushed arguments
  popa                    ; <-- Restore registers
  sti
  iret
%endmacro

%assign i 0
%rep 16
  IRQ_STUB i
  %assign i i + 1
%endrep

global isr80_stub
isr80_stub:
    cli
    pusha                ; save general purpose registers
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10         ; kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr80_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret



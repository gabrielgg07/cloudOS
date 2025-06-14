[bits 32]
[extern irq_handler]

%macro IRQ_STUB 1
  global irq_stub_%1
irq_stub_%1:
  cli
  push byte 0
  push dword (32 + %1)
  call irq_handler
  add esp, 8
  sti
  iret
%endmacro

%assign i 0
%rep 16
  IRQ_STUB i
  %assign i i + 1
%endrep

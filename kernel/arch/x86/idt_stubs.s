[bits 32]
[extern isr_handler]

%macro ISR_NOERR 1
  global isr_stub_%1
isr_stub_%1:
  cli
  push byte 0         ; fake error code
  push dword %1       ; interrupt number
  call isr_handler
  add esp, 8
  sti
  iret
%endmacro

%assign i 0
%rep 32
  ISR_NOERR i
  %assign i i + 1
%endrep

[bits 32]
[extern isr_handler]
[extern page_fault_handler]

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

global isr_stub_14S

isr_stub_14S:
    cli

    ; error code is already pushed by CPU
    ; EIP, CS, EFLAGS are also already on stack

    ; Copy EIP from stack to argument register (pass manually)
    mov eax, [esp + 4]     ; error_code
    mov ebx, [esp + 8]     ; return EIP
    push ebx               ; push EIP
    push eax               ; push error code

    call page_fault_handler

    add esp, 8             ; clean up pushed args

    sti
    iret

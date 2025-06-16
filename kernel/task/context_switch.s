; void switch_to_task(task_t* next)
[global switch_to_task]
switch_to_task:
    mov esp, [esp + 4]  ; load next->esp into esp
    mov ebp, [esp + 4]  ; load next->ebp into ebp
    ret                 ; jump to next EIP (which is at top of stack)

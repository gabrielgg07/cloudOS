; void switch_to_task(task_t* next)
[global switch_to_task]

switch_to_task:
    mov eax, [esp + 4]  ; get pointer to task
    mov esp, [eax]      ; load task->esp
    popa                ; restore edi, esi, ebp, etc.
    iret                ; jump into saved EIP/CS/EFLAGS

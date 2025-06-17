ENTRY(_start)

SECTIONS
{
  . = 1M;

  kernel_start = .;  
  
  .multiboot :
  {
    *(.multiboot)
  }

  .text :
  {
    *(.text*)
  }

  .rodata :
  {
    *(.rodata*)
  }

  .data :
  {
    *(.data*)
  }

  .bss :
  {
    stack_bottom = .;
    . = . + 0x4000;  /* 16 KB stack */
    stack_top = .;
  }

  kernel_end = .;  /* Add this at the end */
}

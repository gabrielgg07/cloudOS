ENTRY(_start)

SECTIONS
{
  . = 1M;

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
}

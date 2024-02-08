ENTRY(_start)
SECTIONS
{
    . = 1M;
    .text (0x100000) : ALIGN(4096)
    {
        *(.text)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096)
    {
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    .asm : ALIGN(4096)
    {
        *(.asm)
    }

    /DISCARD/ : { *(.interp) *(.dynsym) *(.dynstr) *(.dynamic) *(.gnu.hash) *(.note.gnu.build-id) }
}

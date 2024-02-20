ENTRY(_start)
SECTIONS
{
    . = 1M;
    .text (0x100000) : ALIGN(4096)
    {
        *(.text.entry)
        *(.text.main)
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

    .eh : ALIGN(4096)
    {
        *(.eh_frame) 
        *(.eh_frame_hdr)
    }

    .asm : ALIGN(4096)
    {
        *(.asm)
    }

    /DISCARD/ : { *(.interp) *(.dynsym) *(.dynstr) *(.dynamic) *(.gnu.hash) *(.note.gnu.build-id)  }
}

[BITS 32]
SECTION .text.entry

global _start
extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    
    in al, 0x92     ; Enable A20 line
    or al, 2
    out 0x92, al

    mov al, 00010001b ; Initialize master PIC
    out 0x20, al
    mov al, 0x20
    out 0x21, al
    mov al, 00000001b
    out 0x21, al

    call kernel_main

times 512-($ - $$) db 0

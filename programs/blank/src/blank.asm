[BITS 32]

section .asm 

global _start

_start:

loop:

    call getkey
    push eax
    mov eax, 3
    int 0x80
    add esp, 4

    jmp loop

getkey:
    mov eax, 2 ; Command get key
    int 0x80
    cmp eax, 0
    je getkey
    ret

section .data
message: db 'Hello World!', 0
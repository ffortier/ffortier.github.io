[BITS 32]

section .asm 

global _start
extern main

_start:
    push 0
    push 0
    call main
    jmp $ ; TODO: Handle process exit
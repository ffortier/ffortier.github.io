[BITS 32]
SECTION .asm

global idt_load
global int21h
global no_interrupt
global enable_interrupts
global disable_interrupts

extern no_interrupt_handler
extern int21_handler

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt [ebx]

    pop ebp
    ret

no_interrupt:
    cli
    pushad
    call no_interrupt_handler
    popad
    sti
    iret

int21h:
    cli
    pushad
    call int21_handler
    popad
    sti
    iret

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret
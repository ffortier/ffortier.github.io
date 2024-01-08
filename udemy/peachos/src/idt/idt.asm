section .asm

extern isr80h_handler
extern interrupt_handler

global idt_load
global enable_interrupts
global disable_interrupts
global isr80h_wrapper
global interrupt_pointer_table

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret


idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt [ebx]
    pop ebp    
    ret

%macro interrupt 1
    global int%1
    int%1:
        pushad
        push esp
        push dword %1
        call interrupt_handler
        add esp, 8
        popad
        iret
%endmacro

%assign i 0
%rep 512
    interrupt i
%assign i i+1
%endrep

%macro interrupt_pointer_table_entry 1
    dd int%1
%endmacro

interrupt_pointer_table:
%assign i 0
%rep 512
    interrupt_pointer_table_entry i
%assign i i+1
%endrep

isr80h_wrapper:
    pushad
    push esp

    push eax ; command
    call isr80h_handler
    mov dword[tmp_res], eax
    add esp, 8

    popad
    mov eax, [tmp_res]
    iretd

section .data
tmp_res: dd 0
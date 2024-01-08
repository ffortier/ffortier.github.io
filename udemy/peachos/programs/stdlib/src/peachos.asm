[BITS 32]

section .asm

global print:Function
global peachos_getkey:Function
global peachos_malloc:Function
global peachos_free:Function
global peachos_putchar:Function
global peachos_process_load_start:Function
global peachos_process_get_arguments:Function
global peachos_system:Function

; void print(const char* filename);
print:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 1
    int 0x80
    add esp, 4
    pop ebp
    ret

; char peachos_getkey();
peachos_getkey:
    push ebp
    mov ebp, esp
    mov eax, 2
    int 0x80
    pop ebp
    ret

; void peachos_putchar(char c);
peachos_putchar:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 3
    int 0x80
    add esp, 4
    pop ebp
    ret

; void *peachos_malloc(size_t size);
peachos_malloc:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 4
    int 0x80
    add esp, 4
    pop ebp
    ret

; void peachos_free(void* ptr);
peachos_free:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 5
    int 0x80
    add esp, 4
    pop ebp
    ret

; void peachos_process_load_start(const char* filename);
peachos_process_load_start:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 6
    int 0x80
    add esp, 4
    pop ebp
    ret

; int peachos_system(struct command_argument* arguments);
peachos_system:
    push ebp
    mov ebp, esp
    mov eax, 7
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret

; void peachos_process_get_arguments(struct process_arguments* arguments);
peachos_process_get_arguments:
    push ebp
    mov ebp, esp
    mov eax, 8
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret

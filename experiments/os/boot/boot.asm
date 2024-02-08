ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

times 33 db 0   ; Boot Record

start:
    jmp 0:code_start

code_start:
    cli ; Clear Interrupts
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enable Interrupts

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:load32

gdt_start:
gdt_null:
    dd 0
    dd 0

gdt_code:
    dw 0xffff       ; Segment limit first
    dw 0            ; Base
    db 0            ; Base 
    db 0x9a         ; Access byte
    db 11001111b    ; High 4 bit flags and the low 4 bit flags
    db 0            ; Base

gdt_data:
    dw 0xffff       ; Segment limit first
    dw 0            ; Base
    db 0            ; Base 
    db 0x92         ; Access byte
    db 11001111b    ; High 4 bit flags and the low 4 bit flags
    db 0            ; Base

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x00100000
    call ata_lba_read
    jmp CODE_SEG:0x00100000

ata_lba_read:
    mov ebx, eax ; Backup the LBA

    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xe0
    mov dx, 0x1f6
    out dx, al

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1f2
    out dx, al

    ; Send more bits of the LBA
    mov eax, ebx
    mov dx, 0x1f3
    out dx, al

    ; Send more bits of the LBA
    mov dx, 0x1f4
    mov eax, ebx
    shr eax, 8
    out dx, al

    ; Send Upper 16 bits of the LBA
    mov dx, 0x1f5
    mov eax, ebx
    shr eax, 16
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

    mov ecx, 256
    mov dx, 0x1f0
    rep insw

    pop ecx
    loop .next_sector
    ret

times 510-($ - $$) db 0
dw 0xaa55



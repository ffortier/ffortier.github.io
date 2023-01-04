#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "task/task.h"
#include "io/io.h"
#include "status.h"

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

static INTERRUPT_CALLBACK_FUNCTION interrupt_callbacks[PEACHOS_TOTAL_INTERRUPTS];
static ISR80H_COMMAND isr80h_commands[PEACHOS_MAX_ISR80H_COMMANDS];

extern void *interrupt_pointer_table[PEACHOS_TOTAL_INTERRUPTS];
extern void idt_load(struct idtr_desc *ptr);
extern void isr80h_wrapper();

void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void *address)
{
    struct idt_desc *desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (uint32_t)address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xEE;
    desc->offset_2 = (uint32_t)address >> 16;
}

void interrupt_handler(int interrupt, struct interrupt_frame *frame)
{
    kernel_page();
    INTERRUPT_CALLBACK_FUNCTION func = interrupt_callbacks[interrupt];

    if (func)
    {
        task_current_save_state(frame);
        func();
    }
    
    task_page();
    outb(0x20, 0x20);
}

int idt_register_interrupt_callback(int interrupt, INTERRUPT_CALLBACK_FUNCTION interrupt_callback)
{
    int res = 0;

    CHECK_ARG(interrupt >= 0 && interrupt < PEACHOS_TOTAL_INTERRUPTS);
    interrupt_callbacks[interrupt] = interrupt_callback;

out:
    return res;
}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (uint32_t)idt_descriptors;

    for (int i = 0; i < PEACHOS_TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, interrupt_pointer_table[i]);
    }

    idt_set(0, idt_zero);
    idt_set(0x80, isr80h_wrapper);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}

void isr80h_register_command(int command, ISR80H_COMMAND command_func)
{
    if (command < 0 || command >= PEACHOS_MAX_ISR80H_COMMANDS)
    {
        panic("The command is out of bounds\n");
    }

    if (isr80h_commands[command])
    {
        panic("Your attempting to override and existing command\n");
    }

    isr80h_commands[command] = command_func;
}

void *isr80h_handle_command(int command, struct interrupt_frame *frame)
{
    if (command < 0 || command >= PEACHOS_MAX_ISR80H_COMMANDS)
    {
        return 0;
    }

    ISR80H_COMMAND command_func = isr80h_commands[command];

    if (!command_func)
    {
        return 0;
    }

    return command_func(frame);
}

void *isr80h_handler(int command, struct interrupt_frame *frame)
{
    kernel_page();
    task_current_save_state(frame);
    void *res = isr80h_handle_command(command, frame);
    task_page();
    return res;
}
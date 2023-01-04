#include "misc.h"
#include "task/task.h"
#include "kernel.h"
#include "keyboard/keyboard.h"

void *isr80h_command0_sum(struct interrupt_frame *frame)
{
    int v2 = (int)task_get_stack_item(task_current(), 0);
    int v1 = (int)task_get_stack_item(task_current(), 1);
    return (void *)(v1 + v2);
}

void *isr80h_command1_print(struct interrupt_frame *frame)
{
    char buffer[1024];
    char *ptr = task_get_stack_item(task_current(), 0);
    copy_string_from_task(task_current(), ptr, buffer, sizeof(buffer) - 1);
    print(buffer);
    return 0;
}

void *isr80h_command2_getkey(struct interrupt_frame *frame)
{
    char c = keyboard_pop();
    return (void *)(int)c;
}
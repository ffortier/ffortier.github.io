#include "keyboard.h"
#include "classic.h"
#include "status.h"
#include "kernel.h"
#include "task/process.h"

static struct keyboard *keyboard_list_head;
static struct keyboard *keyboard_list_tail;

void keyboard_init()
{
    keyboard_insert(classic_init());
}

int keyboard_insert(struct keyboard *keyboard)
{
    int res = 0;

    CHECK_ARG(keyboard->init);
    CHECK_ARG(keyboard->next == 0);

    if (keyboard_list_tail)
    {
        keyboard_list_tail->next = keyboard;
        keyboard_list_tail = keyboard;
    }
    else
    {
        keyboard_list_head = keyboard;
        keyboard_list_tail = keyboard;
    }

    CHECK_ERR(keyboard->init());

out:
    return res;
}

static int keyboard_get_tail_index(struct process *process)
{
    return process->keyboard.tail % sizeof(process->keyboard.buffer);
}

void keyboard_backspace(struct process *process)
{
    process->keyboard.tail -= 1;
    int real_index = keyboard_get_tail_index(process);
    process->keyboard.buffer[real_index] = 0;
}

void keyboard_push(char c)
{
    struct process *process = process_current();

    if (process)
    {
        int real_index = keyboard_get_tail_index(process);
        process->keyboard.buffer[real_index] = c;
        process->keyboard.tail++;
    }
}

char keyboard_pop()
{
    struct task *task = task_current();
    if (!task)
    {
        return 0;
    }

    struct process *process = task->process;
    int real_index = process->keyboard.head % sizeof(process->keyboard.buffer);
    char c = process->keyboard.buffer[real_index];

    if (!c)
    {
        return 0;
    }

    process->keyboard.buffer[real_index] = 0;
    process->keyboard.head++;

    return c;
}
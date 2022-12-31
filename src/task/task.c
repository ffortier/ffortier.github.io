#include "task.h"
#include "status.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "kernel.h"
#include "idt/idt.h"
#include "memory/paging/paging.h"
#include "string/string.h"

struct task *current_task = 0;

// task linked-list
struct task *task_tail = 0;
struct task *task_head = 0;

struct task *task_current()
{
    return current_task;
}

void task_run_first_ever_task()
{
    if (!task_head)
    {
        panic("task_run_first_ever_task(): no current task\n");
    }

    task_switch(task_head);
    task_return(&task_head->registers);
}

int task_switch(struct task *task)
{
    current_task = task;
    paging_switch(task->page_directory);
    return 0;
}

static void task_save_state(struct task *task, struct interrupt_frame *frame)
{
    task->registers.ip = frame->ip;
    task->registers.cs = frame->cs;
    task->registers.flags = frame->flags;
    task->registers.esp = frame->esp;
    task->registers.ss = frame->ss;
    task->registers.eax = frame->eax;
    task->registers.ebp = frame->ebp;
    task->registers.ebx = frame->ebx;
    task->registers.ecx = frame->ecx;
    task->registers.edi = frame->edi;
    task->registers.edx = frame->edx;
    task->registers.esi = frame->esi;
}

void task_current_save_state(struct interrupt_frame *frame)
{
    struct task *task = task_current();

    if (!task)
    {
        panic("no task to save\n");
    }

    task_save_state(task, frame);
}

int task_page()
{
    user_registers();
    task_switch(current_task);
    return 0;
}

int task_init(struct task *task, struct process *process)
{
    int res = 0;
    memset(task, 0, sizeof(struct task));

    task->page_directory = paging_new_4gb(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    CHECK(task->page_directory, -EIO);

    task->registers.ip = PEACHOS_PROGRAM_VIRTUAL_ADDRESS;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.cs = USER_CODE_SEGMENT;
    task->registers.esp = PEACHOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START;
    task->process = process;

out:
    return res;
}

struct task *task_get_next()
{
    if (!current_task->next)
    {
        return task_head;
    }
    return current_task->next;
}

static void task_list_remove(struct task *task)
{
    if (task->previous)
    {
        task->previous->next = task->next;
    }
    if (task->next)
    {
        task->next->previous = task->previous;
    }
    if (task == task_head)
    {
        task_head = task->next;
    }
    if (task == task_tail)
    {
        task_tail = task->previous;
    }
    if (task == current_task)
    {
        current_task = task_get_next();
    }
}

void task_free(struct task *task)
{
    paging_free_4gb(task->page_directory);
    task_list_remove(task);
    kfree(task);
}

struct task *task_new(struct process *process)
{
    int res = 0;
    struct task *task = kzalloc(sizeof(struct task));

    CHECK(task, -ENOMEM);
    CHECK_ERR(task_init(task, process));

    if (task_head == 0)
    {
        task_head = task;
        task_tail = task;
        current_task = task;
    }
    else
    {
        task_tail->next = task;
        task->previous = task_tail;
        task_tail = task;
    }
out:
    if (res < 0)
    {
        task_free(task);
        task = 0;
    }

    return task;
}

int copy_string_from_task(struct task *task, void *virtual, void *phys, int max)
{
    int res = 0;
    char *tmp;

    CHECK_ARG(max <= PAGING_PAGE_SIZE);
    CHECK(tmp = kzalloc(max), -ENOMEM);

    uint32_t old_entry = paging_get(task->page_directory, tmp);
    paging_map(task->page_directory, tmp, tmp, PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    strncpy(tmp, virtual, max);
    kernel_page();

    CHECK_ERR(paging_set(task->page_directory, tmp, old_entry));

out:
    if (res < 0)
    {
        kfree(tmp);
    }

    return res;
}

#ifdef testing
static void task_reset()
{
    current_task = 0;

    while (task_head)
    {
        task_free(task_head);
    }
}
#endif
#include "task.h"
#include "status.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"

struct task *current_task = 0;

// task linked-list
struct task *task_tail = 0;
struct task *task_head = 0;

struct task *task_current()
{
    return current_task;
}

int task_init(struct task *task)
{
    int res = 0;
    memset(task, 0, sizeof(struct task));

    task->page_directory = paging_new_4gb(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    CHECK(task->page_directory, -EIO);

    task->registers.ip = PEACHOS_PROGRAM_VIRTUAL_ADDRESS;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.esp = PEACHOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START;

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

struct task *task_new()
{
    int res = 0;
    struct task *task = kzalloc(sizeof(struct task));

    CHECK(task, -ENOMEM);
    CHECK_ERR(task_init(task));

    if (task_head == 0)
    {
        task_head = task;
        task_tail = task;
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
#include "process.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"
#include "task/task.h"
#include "memory/heap/kheap.h"
#include "fs/file.h"
#include "string/string.h"
#include "memory/paging/paging.h"
#include <stdbool.h>

struct process *current_process = 0;
static struct process *processes[PEACHOS_MAX_PROCESSES] = {0};

static void process_init(struct process *process)
{
    memset(process, 0, sizeof(struct process));
}

struct process *process_current()
{
    return current_process;
}

int process_get(int process_id, struct process **process)
{
    int res = 0;

    CHECK_ARG(process_id >= 0 && process_id < PEACHOS_MAX_PROCESSES);

    *process = processes[process_id];
out:
    return res;
}

int process_get_free_slot()
{
    for (int i = 0; i < PEACHOS_MAX_PROCESSES; i++)
    {
        if (processes[i] == 0)
        {
            return i;
        }
    }
    return -EISTKN;
}

static bool process_exists(int process_id)
{
    struct process *process;
    int res = process_get(process_id, &process);
    return res == 0 && process != 0;
}

static int process_load_binary(const char *filename, struct process *process)
{
    int res = 0;
    int fd = fopen(filename, "r");

    CHECK(fd, -EIO);

    struct file_stat stat;

    CHECK_ERR(fstat(fd, &stat));

    void *program_data_ptr = kzalloc(stat.filesize);

    CHECK(program_data_ptr, -ENOMEM);
    CHECK_ERR(fread(program_data_ptr, stat.filesize, 1, fd));

    process->ptr = program_data_ptr;
    process->size = stat.filesize;

out:
    fclose(fd);

    return res;
}

static int process_load_data(const char *filename, struct process *process)
{
    int res = 0;

    CHECK_ERR(process_load_binary(filename, process));

out:
    return res;
}

static int process_map_binary(struct process *process)
{
    void *phys_end = paging_align_address(process->ptr + process->size);

    return paging_map_to(
        process->task->page_directory,
        (void *)PEACHOS_PROGRAM_VIRTUAL_ADDRESS,
        process->ptr,
        phys_end,
        PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE);
}

static int process_map_memory(struct process *process)
{
    int res = 0;

    CHECK_ERR(process_map_binary(process));

out:
    return res;
}

void process_free(struct process *process)
{
    if (process)
    {
        if (process->task)
        {
            task_free(process->task);
        }
        if (process->stack)
        {
            kfree(process->stack);
        }
        if (process->ptr)
        {
            kfree(process->ptr);
        }
        for (int i = 0, len = sizeof(process->allocations) / sizeof(process->allocations[0]); i < len; i++)
        {
            if (process->allocations[i])
            {
                kfree(process->allocations[i]);
            }
        }
        kfree(process);
    }
}

int process_load_for_slot(const char *filename, struct process **process_out, int process_slot)
{
    int res = 0;
    struct process *process;

    CHECK(!process_exists(process_slot), -EISTKN);

    process = kzalloc(sizeof(struct process));

    CHECK(process, -ENOMEM);

    process_init(process);

    CHECK_ERR(process_load_data(filename, process));

    process->stack = kzalloc(PEACHOS_USER_PROGRAM_STACK_SIZE);

    CHECK(process->stack, -ENOMEM);

    NS(strncpy(process->filename, filename, sizeof(process->filename) - 1));
    process->id = process_slot;

    process->task = task_new(process);

    CHECK(process->task, -EIO);
    CHECK_ERR(process_map_memory(process));

    processes[process_slot] = process;
    *process_out = process;

out:
    if (res < 0)
    {
        process_free(process);
    }

    return res;
}

int process_load(const char *filename, struct process **process)
{
    int res = 0;
    int process_slot = process_get_free_slot();
    CHECK(process_slot >= 0, -ENOMEM);
    CHECK_ERR(process_load_for_slot(filename, process, process_slot));

out:
    return res;
}

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
static struct process *processes[PEACHOS_MAX_PROCESSES] = {};

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
    int res = 0;

    CHECK_ERR(paging_map_to(
        process->task->page_directory->directory_entry,
        (void *)PEACHOS_PROGRAM_VIRTUAL_ADDRESS,
        process->ptr, paging_align_address(process->ptr + process->size),
        PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE));

out:
    return res;
}

static int process_map_memory(struct process *process)
{
    int res = 0;

    CHECK_ERR(process_map_binary(process));

out:
    return res;
}

int process_load_for_slot(const char *filename, struct process **process, int process_slot)
{
    int res = 0;
    struct task *task = 0;
    struct process *_process;
    void *process_stack_ptr = 0;

    CHECK(!process_exists(process_slot), -EISTKN);

    _process = kzalloc(sizeof(struct process));

    CHECK(_process, -ENOMEM);

    process_init(_process);

    CHECK_ERR(process_load_data(filename, _process));

    process_stack_ptr = kzalloc(PEACHOS_USER_PROGRAM_STACK_SIZE);

    CHECK(process_stack_ptr, -ENOMEM);

    NS(strncpy)
    (_process->filename, filename, sizeof(_process->filename) - 1);
    _process->stack = process_stack_ptr;
    _process->id = process_slot;

    task = task_new(_process);

    CHECK(task, -EIO);

    _process->task = task;

    CHECK_ERR(process_map_memory(_process));

    *process = _process;
    processes[process_slot] = _process;

out:
    if (res < 0)
    {
        if (process_stack_ptr)
        {
            kfree(process_stack_ptr);
        }
        if (_process)
        {
            if (_process->task)
            {
                task_free(_process->task);
            }
            kfree(_process->ptr);
            kfree(_process);
        }
    }

    return res;
}
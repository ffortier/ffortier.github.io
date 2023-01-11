#include "process.h"
#include "task/task.h"
#include "config.h"
#include "status.h"
#include "task/process.h"

void *isr80h_command6_process_load_start(struct interrupt_frame *frame)
{
    void *filename_ptr = task_get_stack_item(task_current(), 0);
    char filename[PEACHOS_MAX_PATH];

    int res = 0;

    struct process *process = 0;

    CHECK_ERR(copy_string_from_task(task_current(), filename_ptr, filename, sizeof(filename)));
    CHECK_ERR(process_load_switch(filename, &process));
    CHECK_ERR(task_switch(process->task));

    task_return(&process->task->registers);

out:
    (void)res;

    return 0;
}

void *isr80h_command7_invoke_system_command(struct interrupt_frame *frame)
{
    return 0;
}

void *isr80h_command8_get_program_arguments(struct interrupt_frame *frame)
{
    struct process *process = task_current()->process;
    struct process_arguments *arguments = task_virtual_address_to_physical(task_current(), task_get_stack_item(task_current(), 0));

    process_get_arguments(process, &arguments->argc, &arguments->argv);

    return 0;
}

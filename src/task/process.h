#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "config.h"
#include "task.h"

struct process
{
    uint16_t id; // pid
    char filename[PEACHOS_MAX_PATH];
    struct task *task;
    void *allocations[PEACHOS_MAX_PROGRAM_ALLOCATIONS];
    void *ptr;     // Physical pointer to the process memory
    void *stack;   // Pointer to the stack memory
    uint32_t size; // Size of the data pointed to by `ptr`
};

int process_load(const char *filename, struct process **process);
#endif
#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "config.h"
#include "task.h"

struct elf_file;

typedef unsigned char PROCESS_TYPE;
enum
{
    FILE_TYPE_ELF,
    FILE_TYPE_BINARY,
};

struct process_allocation
{
    void *ptr;
    size_t size;
};

struct command_argument
{
    char argument[512];

    struct command_argument *next;
};

struct process_arguments
{
    int argc;
    char **argv;
};

struct process
{
    uint16_t id; // pid
    char filename[PEACHOS_MAX_PATH];
    struct task *task;
    struct process_allocation allocations[PEACHOS_MAX_PROGRAM_ALLOCATIONS];
    PROCESS_TYPE file_type;
    union
    {
        void *ptr; // Physical pointer to the process memory
        struct elf_file *elf_file;
    };
    void *stack;   // Pointer to the stack memory
    uint32_t size; // Size of the data pointed to by `ptr`
    struct keyboard_buffer
    {
        char buffer[PEACHOS_KEYBOARD_BUFFER_SIZE];
        int head;
        int tail;
    } keyboard;

    struct process_arguments arguments;
};

struct process *process_current();
int process_load(const char *filename, struct process **process);
int process_switch(struct process *process);
int process_load_switch(const char *filename, struct process **process);
int process_inject_arguments(struct process *process, struct command_argument *root_argument);

void *process_malloc(struct process *process, size_t size);
void process_free_allocation(struct process *process, void *ptr);
void process_get_arguments(struct process *process, int *argc, char ***argv);
void process_free(struct process *process);
void process_terminate(struct process *process);

#endif
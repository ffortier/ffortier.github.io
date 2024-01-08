#include "elfloader.h"
#include "fs/file.h"
#include "status.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "string/string.h"
#include "kernel.h"
#include "config.h"
#include <stdbool.h>

const char elf_signature[] = {0x7f, 'E', 'L', 'F'};

static bool elf_valid_signature(void *buffer)
{
    return memcmp(buffer, (void *)elf_signature, sizeof(elf_signature)) == 0;
}

static bool elf_valid_class(struct elf_header *header)
{
    return header->e_ident[EI_CLASS] == ELFCLASSNONE || header->e_ident[EI_CLASS] == ELFCLASS32;
}

static bool elf_valid_encoding(struct elf_header *header)
{
    return header->e_ident[EI_DATA] == ELFDATANONE || header->e_ident[EI_DATA] == ELFDATA2LSB;
}

static bool elf_is_executable(struct elf_header *header)
{
    return header->e_type == ET_EXEC && header->e_entry >= PEACHOS_PROGRAM_VIRTUAL_ADDRESS;
}

static bool elf_has_program_header(struct elf_header *header)
{
    return header->e_phoff != 0;
}

void *elf_memory(struct elf_file *file)
{
    return file->elf_memory;
}

struct elf_header *elf_header(struct elf_file *file)
{
    return file->elf_memory;
}

struct elf32_shdr *elf_sheader(struct elf_header *header)
{
    return (struct elf32_shdr *)((int)header + header->e_shoff);
}

struct elf32_phdr *elf_pheader(struct elf_header *header)
{
    if (header->e_phoff == 0)
    {
        return 0;
    }

    return (struct elf32_phdr *)((int)header + header->e_phoff);
}

struct elf32_phdr *elf_program_header(struct elf_header *header, int index)
{
    return &elf_pheader(header)[index];
}

struct elf32_shdr *elf_section(struct elf_header *header, int index)
{
    return &elf_sheader(header)[index];
}

char *elf_str_table(struct elf_header *header)
{
    return (char *)header + elf_section(header, header->e_shstrndx)->sh_offset;
}

void *elf_virtual_base(struct elf_file *file)
{
    return file->virtual_base_address;
}

void *elf_virtual_end(struct elf_file *file)
{
    return file->virtual_end_address;
}

void *elf_physical_base(struct elf_file *file)
{
    return file->physical_base_address;
}

void *elf_physical_end(struct elf_file *file)
{
    return file->physical_end_address;
}

int elf_validate_loaded(struct elf_header *header)
{
    int res = 0;

    CHECK(elf_valid_signature(header), -EBADFORMAT);
    CHECK(elf_valid_class(header), -EBADFORMAT);
    CHECK(elf_valid_encoding(header), -EBADFORMAT);
    CHECK(elf_has_program_header(header), -EBADFORMAT);
    CHECK(elf_is_executable(header), -EBADFORMAT);

out:
    return res;
}

int elf_process_phdr_load(struct elf_file *elf_file, struct elf32_phdr *phdr)
{
    if (elf_file->virtual_base_address >= (void *)phdr->p_vaddr || !elf_file->virtual_base_address)
    {
        elf_file->virtual_base_address = (void *)phdr->p_vaddr;
        elf_file->physical_base_address = (void *)((int)elf_memory(elf_file) + phdr->p_offset);
    }

    unsigned int end_virtual_address = phdr->p_vaddr + phdr->p_filesz;

    if (elf_file->virtual_end_address <= (void *)(end_virtual_address || !elf_file->virtual_end_address))
    {
        elf_file->virtual_end_address = (void *)end_virtual_address;
        elf_file->physical_end_address = (void *)((int)elf_memory(elf_file) + phdr->p_offset + phdr->p_filesz);
    }

    return 0;
}

int elf_process_pheader(struct elf_file *elf_file, struct elf32_phdr *phdr)
{
    int res = 0;

    switch (phdr->p_type)
    {
    case PT_LOAD:
        res = elf_process_phdr_load(elf_file, phdr);
        break;
    }

    return res;
}

int elf_process_pheaders(struct elf_file *elf_file)
{
    int res = 0;

    struct elf_header *header = elf_header(elf_file);

    for (int i = 0; i < header->e_phnum; i++)
    {
        struct elf32_phdr *phdr = elf_program_header(header, i);

        CHECK_ERR(elf_process_pheader(elf_file, phdr));
    }

out:
    return res;
}

int elf_process_loaded(struct elf_file *elf_file)
{
    int res = 0;
    struct elf_header *header = elf_header(elf_file);

    CHECK_ERR(elf_validate_loaded(header));
    CHECK_ERR(elf_process_pheaders(elf_file));

out:
    return res;
}

void elf_close(struct elf_file *elf_file)
{
    if (elf_file)
    {
        if (elf_file->elf_memory)
        {
            kfree(elf_file->elf_memory);
        }
        kfree(elf_file);
    }
}

int elf_load(const char *filename, struct elf_file **file_out)
{
    struct elf_file *elf_file = kzalloc(sizeof(struct elf_file));
    int res = 0;
    int fd = fopen(filename, "r");
    struct file_stat stat;

    CHECK_ERR(fd);
    CHECK_ERR(fstat(fd, &stat));

    elf_file->elf_memory = kzalloc(stat.filesize);

    CHECK_ERR(fread(elf_file->elf_memory, stat.filesize, 1, fd));
    CHECK_ERR(elf_process_loaded(elf_file));
    *file_out = elf_file;

out:
    if (fd >= 0)
    {
        fclose(fd);
    }

    if (res < 0)
    {
        elf_close(elf_file);
    }

    return res;
}

void *elf_phdr_phys_addr(struct elf_file *elf_file, struct elf32_phdr *phdr)
{
    return elf_memory(elf_file) + phdr->p_offset;
}
#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"

void paging_load_directory(uint32_t *directory);

static uint32_t *current_directory;

static bool paging_is_aligned(void *addr)
{
    return (intptr_t)addr % PAGING_PAGE_SIZE == 0;
}

static int paging_get_indexes(void *virtual_address, uint32_t *directory_index_out, uint32_t *table_index_out)
{
    if (!paging_is_aligned(virtual_address))
    {
        return -EINVARG;
    }

    *directory_index_out = ((uint32_t)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *table_index_out = ((uint32_t)virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);

    return OK;
}

int paging_set(struct paging_4gb_chunk *directory, void *virtual_address, uint32_t val)
{
    if (!paging_is_aligned(virtual_address))
    {
        return -EINVARG;
    }

    uint32_t directory_index = 0;
    uint32_t table_index = 0;

    int res = paging_get_indexes(virtual_address, &directory_index, &table_index);

    if (res < 0)
    {
        return res;
    }

    uint32_t entry = directory->directory_entry[directory_index];
    uint32_t *table = (uint32_t *)(entry & 0xfffff000);

    table[table_index] = val;

    return OK;
}

struct paging_4gb_chunk *paging_new_4gb(uint8_t flags)
{
    uint32_t *directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    int offset = 0;

    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        uint32_t *entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for (int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++)
        {
            entry[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
        }
        offset += PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITEABLE;
    }

    struct paging_4gb_chunk *chunk = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk->directory_entry = directory;
    return chunk;
}

void paging_switch(struct paging_4gb_chunk *chunk)
{
    paging_load_directory(chunk->directory_entry);
    current_directory = chunk->directory_entry;
}

void paging_free_4gb(struct paging_4gb_chunk *chunk)
{
    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        uint32_t entry = chunk->directory_entry[i];
        uint32_t *table = (uint32_t *)(uintptr_t)(entry & 0xfffff000);
        kfree(table);
    }
    kfree(chunk->directory_entry);
    kfree(chunk);
}

int paging_map(struct paging_4gb_chunk *directory, void *virt, void *phys, int flags)
{
    if (((unsigned int)virt % PAGING_PAGE_SIZE) || ((unsigned int)phys % PAGING_PAGE_SIZE))
    {
        return -EINVARG;
    }
    return paging_set(directory, virt, (uint32_t)phys | flags);
}

int paging_map_range(struct paging_4gb_chunk *directory, void *virt, void *phys, int count, int flags)
{
    int res = 0;
    for (int i = 0; i < count; i++)
    {
        CHECK_ERR(paging_map(directory, virt, phys, flags));
        virt += PAGING_PAGE_SIZE;
        phys += PAGING_PAGE_SIZE;
    }
out:
    return res;
}

int paging_map_to(struct paging_4gb_chunk *directory, void *virt, void *phys, void *phys_end, int flags)
{
    int res = 0;

    CHECK_ARG((uintptr_t)virt % PAGING_PAGE_SIZE == 0);
    CHECK_ARG((uintptr_t)phys % PAGING_PAGE_SIZE == 0);
    CHECK_ARG((uintptr_t)phys_end % PAGING_PAGE_SIZE == 0);
    CHECK_ARG((uintptr_t)phys_end >= (uintptr_t)phys);

    uint32_t total_bytes = phys_end - phys;
    int total_pages = total_bytes / PAGING_PAGE_SIZE;
    CHECK_ERR(paging_map_range(directory, virt, phys, total_pages, flags));

out:
    return res;
}

void *paging_align_address(void *ptr)
{
    if ((uint32_t)ptr % PAGING_PAGE_SIZE)
    {
        return (void *)((uint32_t)ptr + PAGING_PAGE_SIZE - ((uint32_t)ptr % PAGING_PAGE_SIZE));
    }

    return ptr;
}

uint32_t paging_get(struct paging_4gb_chunk *directory, void *virt)
{
    uint32_t dir_index = 0;
    uint32_t table_index = 0;
    paging_get_indexes(virt, &dir_index, &table_index);
    uint32_t entry = directory->directory_entry[dir_index];
    uint32_t *table = (uint32_t *)(entry & 0xfffff000);
    return table[table_index];
}
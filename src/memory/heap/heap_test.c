#include "heap.h"
#include "testsupport.h"
#include <stdio.h>
#include <stdlib.h>

#include "heap.c"

#define TOTAL_BLOCKS 8
#define BLOCK_SIZE 4096

struct test_memory
{
    uint8_t *buffer;
    void *saddr;
    void *eaddr;
};

struct test_memory *memory;
struct heap *heap;
struct heap_table *table;

static void setup()
{
    memory = malloc(sizeof(struct test_memory));
    memory->buffer = malloc(TOTAL_BLOCKS * BLOCK_SIZE + 1);
    memory->saddr = memory->buffer + BLOCK_SIZE - ((uintptr_t)memory->buffer % BLOCK_SIZE);
    memory->eaddr = memory->saddr + TOTAL_BLOCKS * BLOCK_SIZE;

    table = malloc(sizeof(struct heap_table));
    heap = malloc(sizeof(struct heap));

    table->total = TOTAL_BLOCKS;
    table->entries = malloc(sizeof(HEAP_BLOCK_TABLE_ENTRY) * TOTAL_BLOCKS);
}

static void teardown()
{
    free(memory->buffer);
    free(memory);
    free(table->entries);
    free(table);
    free(heap);
}

static bool all_zeros(uint8_t *saddr, uint8_t *eaddr)
{
    while (saddr != eaddr)
    {
        if (*saddr++ != 0)
        {
            return false;
        }
    }
    return true;
}

static bool all_free(struct heap_table *table)
{
    return all_zeros(table->entries, table->entries + table->total);
}

TEST_CASE(heap)
{
    setup();

    int res = heap_create(heap, memory->saddr, memory->eaddr, table);

    EXPECT(res == 0);
    EXPECT(all_free(table));

    teardown();
}

TEST_SUITE(TEST_REF(heap));

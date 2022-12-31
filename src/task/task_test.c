#define USE_TEARDOWN

#include "testsupport.h"
#include "task.c"

struct process
{
};

struct paging_4gb_chunk test_chunk;
struct process process = {};

struct paging_4gb_chunk *paging_new_4gb(uint8_t flags)
{
    return &test_chunk;
}

void paging_free_4gb(struct paging_4gb_chunk *chunk) {}
void task_return(struct registers *regs) {}
void user_registers() {}
void paging_switch(struct paging_4gb_chunk *chunk) {}
void panic(const char *msg) {}

TEARDOWN
{
    task_reset();
}

TEST_CASE(task_current_empty)
{
    struct task *task = task_current();
    EXPECT(task == 0);
}

TEST_CASE(task_linked_list)
{
    struct task *t1 = task_new(&process);
    struct task *t2 = task_new(&process);
    struct task *t3 = task_new(&process);
    struct task *t4 = task_new(&process);
    task_free(t2);
    EXPECT(t1 == task_head);
    EXPECT(t1->next == t3);
    EXPECT(t1->next->next == t4);
    EXPECT(t1->next->next->next == 0);
    EXPECT(t4->previous == t3);
    EXPECT(t4->previous->previous == t1);
    EXPECT(t4->previous->previous->previous == 0);
    EXPECT(t4 == task_tail);
}

TEST_CASE(task_free_head)
{
    struct task *t1 = task_new(&process);
    struct task *t2 = task_new(&process);
    struct task *t3 = task_new(&process);
    task_free(t1);
    EXPECT(t2 == task_head);
    EXPECT(t2->previous == 0);
}

TEST_CASE(task_free_tail)
{
    struct task *t1 = task_new(&process);
    struct task *t2 = task_new(&process);
    struct task *t3 = task_new(&process);
    task_free(t3);
    EXPECT(t2 == task_tail);
    EXPECT(t2->next == 0);
}

TEST_SUITE(
    TEST_REF(task_current_empty),
    TEST_REF(task_linked_list),
    TEST_REF(task_free_head),
    TEST_REF(task_free_tail),

);
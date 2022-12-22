#include "testsupport.h"
#include "memory/paging/paging.c"
#include "task.c"

TEST_CASE(task_current_empty)
{
    struct task *task = task_current();
    EXPECT(task == 0);
}

TEST_SUITE(
    TEST_REF(task_current_empty),

);
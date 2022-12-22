#ifndef TESTSUPPORT_H
#define TESTSUPPORT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*test_case)(bool *);

struct test_ref
{
    test_case fn;
    const char *description;
};

#define TEST_SUITE(tests...)                                        \
    void *kzalloc(size_t size)                                      \
    {                                                               \
        return malloc(size);                                        \
    }                                                               \
                                                                    \
    void kfree(void *ptr)                                           \
    {                                                               \
        return free(ptr);                                           \
    }                                                               \
                                                                    \
    int main()                                                      \
    {                                                               \
        struct test_ref refs[] = {tests};                           \
        int len = sizeof(refs) / sizeof(struct test_ref);           \
        int total_failures = 0;                                     \
                                                                    \
        for (int i = 0; i < len; i++)                               \
        {                                                           \
            bool fail = false;                                      \
            (*refs[i].fn)(&fail);                                   \
            if (fail)                                               \
            {                                                       \
                total_failures++;                                   \
                fprintf(stderr, "FAIL: %s\n", refs[i].description); \
            }                                                       \
            else                                                    \
            {                                                       \
                fprintf(stderr, "PASS: %s\n", refs[i].description); \
            }                                                       \
        }                                                           \
        return total_failures;                                      \
    }

#define TEST_REF(desc)                           \
    {                                            \
        .fn = &test_##desc, .description = #desc \
    }

#define TEST_CASE(desc) void test_##desc(bool *fail)

#define EXPECT(condition)                                            \
    {                                                                \
        *fail = !(condition);                                        \
        if (*fail)                                                   \
        {                                                            \
            fprintf(stderr, "Expectation failed: %s\n", #condition); \
            return;                                                  \
        }                                                            \
    }

#endif

#ifndef TESTSUPPORT_H
#define TESTSUPPORT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TEARDOWN void teardown()
#define SETUP void setup()

#ifndef USE_TEARDOWN
TEARDOWN
{
}
#else
TEARDOWN;
#endif

#ifndef USE_SETUP
SETUP
{
}
#else
SETUP;
#endif

typedef void (*test_case)(bool *, char **);

struct test_ref
{
    test_case fn;
    const char *description;
};

int allocation_count = 0;

void *kzalloc(size_t size)
{
    allocation_count += 1;
    return malloc(size);
}

void kfree(void *ptr)
{
    allocation_count -= 1;
    return free(ptr);
}

#define TEST_SUITE(tests...)                                                                                                   \
    int main()                                                                                                                 \
    {                                                                                                                          \
        char *messages[1024];                                                                                                  \
        int message_index = 0;                                                                                                 \
        struct test_ref refs[] = {tests};                                                                                      \
        int len = sizeof(refs) / sizeof(struct test_ref);                                                                      \
        int total_failures = 0;                                                                                                \
                                                                                                                               \
        for (int i = 0; i < len; i++)                                                                                          \
        {                                                                                                                      \
            allocation_count = 0;                                                                                              \
            char *msg = 0;                                                                                                     \
            bool fail = false;                                                                                                 \
            setup();                                                                                                           \
            (*refs[i].fn)(&fail, &msg);                                                                                        \
            teardown();                                                                                                        \
            char *c = ".";                                                                                                     \
            if (fail)                                                                                                          \
            {                                                                                                                  \
                total_failures++;                                                                                              \
                messages[message_index] = malloc(sizeof(char) * 1024);                                                         \
                sprintf(messages[message_index++], "FAIL: %s in file %s", refs[i].description, __FILE__);                      \
                c = "X";                                                                                                       \
            }                                                                                                                  \
            if (msg)                                                                                                           \
            {                                                                                                                  \
                messages[message_index] = msg;                                                                                 \
            }                                                                                                                  \
            if (allocation_count)                                                                                              \
            {                                                                                                                  \
                messages[message_index] = malloc(sizeof(char) * 1024);                                                         \
                sprintf(messages[message_index++], "WARN: Possible memory leak %s in file %s", refs[i].description, __FILE__); \
            }                                                                                                                  \
            fprintf(stdout, "%s", c);                                                                                          \
        }                                                                                                                      \
        fprintf(stdout, "\n");                                                                                                 \
        for (int i = 0; i < message_index; i++)                                                                                \
        {                                                                                                                      \
            fprintf(stderr, "%s\n", messages[i]);                                                                              \
            free(messages[i]);                                                                                                 \
        }                                                                                                                      \
        return total_failures;                                                                                                 \
    }

#define TEST_REF(desc)                           \
    {                                            \
        .fn = &test_##desc, .description = #desc \
    }

#define TEST_CASE(desc) void test_##desc(bool *fail, char **message)

#define EXPECT(condition)                                            \
    {                                                                \
        *fail = !(condition);                                        \
        if (*fail)                                                   \
        {                                                            \
            *message = malloc(sizeof(char) * 1024);                  \
            sprintf(*message, "Expectation failed: %s", #condition); \
            return;                                                  \
        }                                                            \
    }

#endif

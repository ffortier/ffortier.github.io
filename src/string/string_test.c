#include "testsupport.h"
#include "string.c"
#include <stdio.h>

TEST_CASE(strlen_empty)
{
    int value = NS(strlen)("");
    EXPECT(value == 0);
}

TEST_CASE(strlen_not_empty)
{
    int value = NS(strlen)("hello world");
    EXPECT(value == 11);
}

TEST_CASE(strnlen_empty)
{
    int value = NS(strnlen)("", 15);
    EXPECT(value == 0);
}

TEST_CASE(strnlen_not_empty)
{
    int value = NS(strnlen)("hello world", 15);
    EXPECT(value == 11);
}

TEST_CASE(strnlen_max_reached)
{
    int value = NS(strnlen)("hello world longer than 15 chars", 15);
    EXPECT(value == 15);
}

TEST_CASE(strcpy)
{
    char buf[1024];
    NS(strcpy)
    (buf, "hello world");
    EXPECT(NS(strncmp)(buf, "hello world", sizeof(buf) - 1) == 0);
}

TEST_SUITE(
    TEST_REF(strlen_empty),
    TEST_REF(strlen_not_empty),
    TEST_REF(strnlen_empty),
    TEST_REF(strnlen_not_empty),
    TEST_REF(strnlen_max_reached),
    TEST_REF(strcpy),

);
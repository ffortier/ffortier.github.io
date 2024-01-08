#include "testsupport.h"
#include "stdlib.c"
#include <stdio.h>
#include <string.h>

TEST_CASE(itoa_positive_number)
{
    char *str = itoa(42);

    EXPECT(strncmp(str, "42", 2) == 0);
}

TEST_CASE(itoa_negative_number)
{
    char *str = itoa(-42);

    EXPECT(strncmp(str, "-42", 4) == 0);
}

TEST_CASE(itoa_zero)
{
    char *str = itoa(0);

    EXPECT(strncmp(str, "0", 2) == 0);
}

TEST_SUITE(
    TEST_REF(itoa_positive_number),
    TEST_REF(itoa_negative_number),
    TEST_REF(itoa_zero),

);

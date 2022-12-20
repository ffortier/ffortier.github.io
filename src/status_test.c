#include "status.h"
#include "testsupport.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static int echo(int value, int *call_count)
{
    *call_count += 1;
    return value;
}

TEST_CASE(check_macro_success)
{
    int i = 0;
    int res = 0;

    check(i++ == 0, -EIO);

out:
    EXPECT(res == 0);
    EXPECT(i == 1);
}

TEST_CASE(check_macro_fail)
{
    int i = 0;
    int res = 0;

    check(i++ == 1, -EIO);

out:
    EXPECT(res == -EIO);
    EXPECT(i == 1);
}

TEST_CASE(check_arg_macro_success)
{
    int i = 0;
    int res = 0;

    check_arg(i++ == 0);

out:
    EXPECT(res == 0);
    EXPECT(i == 1);
}

TEST_CASE(check_arg_macro_fail)
{
    int i = 0;
    int res = 0;

    check_arg(i++ == 1);

out:
    EXPECT(res == -EINVARG);
    EXPECT(i == 1);
}

TEST_CASE(check_err_macro_success)
{
    int i = 0;
    int res = 0;

    check_err(echo(OK, &i));

out:
    EXPECT(res == 0);
    EXPECT(i == 1);
}

TEST_CASE(check_err_macro_fail)
{
    int i = 0;
    int res = 0;

    check_err(echo(-ENOMEM, &i));

out:
    EXPECT(res == -ENOMEM);
    EXPECT(i == 1);
}

TEST_SUITE(
    TEST_REF(check_macro_success),
    TEST_REF(check_macro_fail),
    TEST_REF(check_arg_macro_success),
    TEST_REF(check_arg_macro_fail),
    TEST_REF(check_err_macro_success),
    TEST_REF(check_err_macro_fail),

);
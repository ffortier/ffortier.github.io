#include "pparser.c"
#include "testsupport.h"
#include <stdlib.h>
#include <stdio.h>

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int to_num_digit(char c)
{
    return c - 48;
}

TEST_CASE(non_empty_path)
{
    struct path_root *path_root = pathparser_parse("0:/some/path.txt", "");

    EXPECT(path_root != 0);
    EXPECT(path_root->drive_no == 0);
    EXPECT(memcmp(path_root->first->part, "some", 5) == 0);
    EXPECT(memcmp(path_root->first->next->part, "path.txt", 9) == 0);
    EXPECT(path_root->first->next->next == 0);
}

TEST_CASE(empty_path)
{
    struct path_root *path_root = pathparser_parse("0:/", "");

    EXPECT(path_root != 0);
    EXPECT(path_root->drive_no == 0);
    EXPECT(memcmp(path_root->first->part, "", 1) == 0);
    EXPECT(path_root->first->next == 0);
}

TEST_CASE(invalid_path)
{
    struct path_root *path_root = pathparser_parse("patate", "");

    EXPECT(path_root == 0);
}

TEST_SUITE(
    TEST_REF(non_empty_path),
    TEST_REF(empty_path),
    TEST_REF(invalid_path))

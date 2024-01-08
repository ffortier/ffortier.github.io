#include "pparser.h"
#include "config.h"
#include "status.h"
#include <stdbool.h>

#ifndef testing
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#define KZALLOC(size) kzalloc(size)
#define KFREE(ptr) kfree(ptr)
#else
#include <stdlib.h>
#include <string.h>
#define KZALLOC(size) calloc(1, size)
#define KFREE(ptr) free(ptr)
bool is_digit(char c);
int to_num_digit(char c);
#endif

void pathparser_free(struct path_root *root)
{
    KFREE(root);
}

static bool pathparser_validate_path(const char *path)
{
    return (is_digit(*path) && memcmp(&path[1], ":/", 2) == 0);
}

static int pathparser_analyze_path(const char *path, size_t *part_count, size_t *path_len)
{
    if (!pathparser_validate_path(path))
    {
        return -EBADPATH;
    }

    path += 3;

    *part_count = 1;
    *path_len = 0;

    while (*path++)
    {
        if (*path == '/')
        {
            *part_count += 1;
        }

        *path_len += 1;
    }

    if (*path_len >= PEACHOS_MAX_PATH)
    {
        return -EBADPATH;
    }

    return 0;
}

static char *pathparser_get_path_part(char **path)
{
    char *start = *path;

    while (**path != '/' && **path != 0)
    {
        *path += 1;
    }

    if (**path == '/')
    {
        **path = '\0';
        *path += 1;
    }

    return start;
}

static void pathparser_parse_path_part(struct path_part *last, char **path, struct path_part *next)
{
    next->part = pathparser_get_path_part(path);
    next->next = 0;

    if (last)
    {
        last->next = next;
    }
}

static struct path_part *pathparser_parse_path_parts(struct path_part *first, size_t part_count, char *path)
{
    struct path_part *last = 0;
    struct path_part *next = first;

    for (int i = 0; i < part_count; i++)
    {
        pathparser_parse_path_part(last, &path, next);
        last = next;
        next++;
    }

    return first;
}

struct path_root *pathparser_parse(const char *path, const char *current_directory_path)
{
    size_t part_count;
    size_t path_len;

    if (pathparser_analyze_path(path, &part_count, &path_len) < 0)
    {
        return 0;
    }

    size_t path_offset = sizeof(struct path_root) + sizeof(struct path_part) * part_count;
    size_t mem_size = path_offset + path_len + 1;

    void *ptr = KZALLOC(mem_size);

    if (!ptr)
    {
        return 0;
    }

    char *path_copy = ptr + path_offset;

    strcpy(path_copy, &path[3]);

    struct path_root *root = ptr;

    root->drive_no = to_num_digit(path[0]);
    root->first = pathparser_parse_path_parts(ptr + sizeof(struct path_root), part_count, path_copy);

    return root;
}

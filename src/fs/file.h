#ifndef FILE_H
#define FILE_H

#include "pparser.h"
#include <stdint.h>

typedef unsigned int FILE_SEEK_MODE;
#ifndef _STDIO_H
enum
{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};
#endif

typedef unsigned int FILE_MODE;
enum
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

struct disk;

typedef void *(*FS_OPEN_FUNCTION)(struct disk *disk, struct path_root *root, FILE_MODE mode);
typedef int (*FS_READ_FUNCTION)(struct disk *disk, void *private, uint32_t size, uint32_t nmemb, char *out);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk *disk);

struct filesystem
{
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;
    FS_READ_FUNCTION read;

    char name[20];
};

struct file_descriptor
{
    int index;
    struct filesystem *filesystem;
    void *private_data;
    struct disk *disk;
};

void fs_init();

#ifndef testing
int fopen(const char *filename, const char *mode);
int fread(void *ptr, uint32_t size, uint32_t nmemb, int fd);
#endif

void fs_insert_filesystem(struct filesystem *filesystem);
struct filesystem *fs_resolve(struct disk *disk);
#endif
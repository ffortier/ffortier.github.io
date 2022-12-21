#include <stdio.h>
#include <stdlib.h>
#include "testsupport.h"
#include "string/string.c"
#include "fat16.c"
#include "memory/heap/heap.h"

void *kzalloc(size_t size)
{
    return malloc(size);
}

void kfree(void *ptr)
{
    return free(ptr);
}

int diskstreamer_read(struct disk_stream *stream, void *out, int total)
{
    FILE *fd = fopen("./bin/vfat16.bin", "r");
    fseek(fd, stream->pos, SEEK_SET);
    fread(out, 1, total, fd);
    fclose(fd);
    stream->pos += total;
    return 0;
}

int diskstreamer_seek(struct disk_stream *stream, int pos)
{
    stream->pos = pos;
    return 0;
}

struct disk_stream *diskstreamer_new(int disk_id)
{
    struct disk_stream *stream = malloc(sizeof(struct disk_stream));
    stream->pos = 0;
    stream->disk = 0;
    return stream;
}

void diskstream_close(struct disk_stream *stream)
{
    free(stream);
}

TEST_CASE(open_file_at_root)
{
    struct disk disk = {.id = 0, .sector_size = 512};
    struct path_part first = {.next = 0, .part = "hello.txt"};
    struct path_root root = {.drive_no = 0, .first = &first};
    void *private = 0;
    fat16_resolve(&disk);
    EXPECT(disk.filesystem != 0);
    EXPECT(fat16_open(&disk, &root, FILE_MODE_READ, &private) == 0);
    EXPECT(fat16_close(private) == 0);
}

TEST_CASE(open_file_at_root_no_ext)
{
    struct disk disk = {.id = 0, .sector_size = 512};
    struct path_part first = {.next = 0, .part = "noext"};
    struct path_root root = {.drive_no = 0, .first = &first};
    void *private = 0;
    fat16_resolve(&disk);
    EXPECT(disk.filesystem != 0);
    EXPECT(fat16_open(&disk, &root, FILE_MODE_READ, &private) == 0);
    EXPECT(fat16_close(private) == 0);
}

TEST_CASE(open_file_in_subfolder)
{
    struct disk disk = {.id = 0, .sector_size = 512};
    struct path_part third = {.next = 0, .part = "hi.txt"};
    struct path_part second = {.next = &third, .part = "dir"};
    struct path_part first = {.next = &second, .part = "some"};
    struct path_root root = {.drive_no = 0, .first = &first};
    void *private = 0;
    fat16_resolve(&disk);
    EXPECT(disk.filesystem != 0);
    EXPECT(fat16_open(&disk, &root, FILE_MODE_READ, &private) == 0);
    EXPECT(fat16_close(private) == 0);
}

TEST_CASE(open_file_not_found)
{
    struct disk disk = {.id = 0, .sector_size = 512};
    struct path_part third = {.next = 0, .part = "hi.txt"};
    struct path_part second = {.next = &third, .part = "dirrrr"};
    struct path_part first = {.next = &second, .part = "some"};
    struct path_root root = {.drive_no = 0, .first = &first};
    void *private = 0;
    fat16_resolve(&disk);
    EXPECT(disk.filesystem != 0);
    EXPECT(fat16_open(&disk, &root, FILE_MODE_READ, &private) == 0);
    EXPECT(fat16_close(private) == 0);
}

TEST_CASE(open_file_at_root_not_found)
{
    struct disk disk = {.id = 0, .sector_size = 512};
    struct path_part first = {.next = 0, .part = "hellooo.txt"};
    struct path_root root = {.drive_no = 0, .first = &first};
    void *private = 0;
    fat16_resolve(&disk);
    EXPECT(disk.filesystem != 0);
    EXPECT(fat16_open(&disk, &root, FILE_MODE_READ, &private) == 0);
    EXPECT(fat16_close(private) == 0);
}

TEST_SUITE(
    TEST_REF(open_file_at_root),
    TEST_REF(open_file_at_root_no_ext),
    TEST_REF(open_file_in_subfolder),
    TEST_REF(open_file_not_found),
    TEST_REF(open_file_at_root_not_found),

);
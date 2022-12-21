#include "file.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "kernel.h"
#include "fs/fat/fat16.h"
#include "disk/disk.h"
#include "string/string.h"

struct filesystem *filesystems[PEACHOS_MAX_FILESYSTEMS];
struct file_descriptor *file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

static struct filesystem **fs_get_free_filesystem()
{
    for (int i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] == 0)
        {
            return &filesystems[i];
        }
    }
    return 0;
}

void fs_insert_filesystem(struct filesystem *filesystem)
{
    struct filesystem **fs;
    fs = fs_get_free_filesystem();

    if (!fs)
    {
        panic("Problem inserting filesystem");
    }

    *fs = filesystem;
}

static void fs_static_load()
{
    fs_insert_filesystem(fat16_init());
}

void fs_load()
{
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init()
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor **desc_out)
{
    for (int i = 0; i < PEACHOS_MAX_FILE_DESCRIPTORS; i++)
    {
        if (file_descriptors[i] == 0)
        {
            struct file_descriptor *desc = kzalloc(sizeof(struct file_descriptor));

            desc->index = i + 1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            return 0;
        }
    }

    return -ENOMEM;
}

static struct file_descriptor *file_get_descriptor(int fd)
{
    if (fd <= 0 || fd > PEACHOS_MAX_FILE_DESCRIPTORS)
    {
        return 0;
    }

    int index = fd - 1;

    return file_descriptors[index];
}

struct filesystem *fs_resolve(struct disk *disk)
{
    for (int i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0)
        {
            return filesystems[i];
        }
    }
    return 0;
}

FILE_MODE file_get_mode_by_string(const char *str)
{
    if (strncmp(str, "r", 1) == 0)
    {
        return FILE_MODE_READ;
    }

    if (strncmp(str, "w", 1) == 0)
    {
        return FILE_MODE_WRITE;
    }

    if (strncmp(str, "a", 1) == 0)
    {
        return FILE_MODE_APPEND;
    }

    return FILE_MODE_INVALID;
}

int fopen(const char *filename, const char *mode_str)
{
    int res = 0;
    struct path_root *root = pathparser_parse(filename, NULL);

    check_arg(root);
    check_arg(root->first && root->first->part[0] != '\0');

    struct disk *disk = disk_get(root->drive_no);

    check(disk, -EIO);
    check(disk->filesystem, -EIO);

    FILE_MODE mode = file_get_mode_by_string(mode_str);

    check_arg(mode != FILE_MODE_INVALID);

    void *descriptor_private_data = 0;
    check_err(disk->filesystem->open(disk, root, mode, &descriptor_private_data));

    struct file_descriptor *desc = 0;
    check_err(file_new_descriptor(&desc));

    desc->filesystem = disk->filesystem;
    desc->private_data = descriptor_private_data;
    desc->disk = disk;

    res = desc->index;
out:
    if (res < 0)
    {
        if (root)
        {
            kfree(root);
        }
        res = 0;
    }

    return res;
}

int fread(void *ptr, uint32_t size, uint32_t nmemb, int fd)
{
    int res = OK;
    check_arg(size > 0 && nmemb > 0 && fd >= 0);
    struct file_descriptor *desc = file_get_descriptor(fd);
    check_arg(desc);
    res = desc->filesystem->read(desc->disk, desc->private_data, size, nmemb, (char *)ptr);

out:
    return res;
}

int fseek(int fd, uint32_t offset, FILE_SEEK_MODE whence)
{
    int res = OK;
    struct file_descriptor *desc = file_get_descriptor(fd);
    check_arg(desc);
    res = desc->filesystem->seek(desc->private_data, offset, whence);

out:
    return res;
}

int fstat(int fd, struct file_stat *stat)
{
    int res = OK;
    struct file_descriptor *desc = file_get_descriptor(fd);
    check_arg(desc);
    res = desc->filesystem->stat(desc->disk, desc->private_data, stat);

out:
    return res;
}

int fclose(int fd)
{
    int res = OK;
    struct file_descriptor *desc = file_get_descriptor(fd);
    check_arg(desc);
    check_err(desc->filesystem->close(desc->private_data));
    kfree(desc);
    file_descriptors[fd - 1] = 0;

out:
    return res;
}
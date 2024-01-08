#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

#include "disk/disk.h"

struct disk_stream
{
    int pos;
    struct disk *disk;
};

int diskstreamer_read(struct disk_stream *stream, void *out, int total);
int diskstreamer_seek(struct disk_stream *stream, int pos);
struct disk_stream *diskstreamer_new(int disk_id);
void diskstream_close(struct disk_stream *stream);

#endif
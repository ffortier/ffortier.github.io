#ifndef PPARSER_H
#define PPARSER_H

struct path_root
{
    int drive_no;
    struct path_part *first;
};

struct path_part
{
    char *part;
    struct path_part *next;
};

void pathparser_free(struct path_root *root);
struct path_root *pathparser_parse(const char *path, const char *current_directory_path);

#endif
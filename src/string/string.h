#ifndef PEACHOS_STRING_H
#define PEACHOS_STRING_H

#include <stddef.h>
#include <stdbool.h>
#include "config.h"

size_t NS(strlen)(const char *str);
size_t NS(strnlen)(const char *str, size_t max);
char *NS(strcpy)(char *buf, const char *str);
char *NS(strncpy)(char *buf, const char *str, size_t max);
bool is_digit(char c);
int to_num_digit(char c);
int NS(istrncmp)(const char *s1, const char *s2, size_t n);
char to_lower(char c);
int strnlen_terminator(const char *str, int max, char terminator);
int NS(strncmp)(const char *str1, const char *str2, size_t n);

#endif
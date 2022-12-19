#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char *str);
size_t strnlen(const char *str, size_t max);
char *strcpy(char *buf, const char *str);
bool is_digit(char c);
int to_num_digit(char c);
int istrncmp(const char *s1, const char *s2, int n);
char to_lower(char c);
int strnlen_terminator(const char *str, int max, char terminator);
int strncmp(const char *str1, const char *str2, int n);

#endif
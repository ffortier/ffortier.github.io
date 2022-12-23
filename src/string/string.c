#include "string.h"
#include "config.h"

size_t NS(strlen)(const char *str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }

    return len;
}

size_t NS(strnlen)(const char *str, size_t max)
{
    size_t len = 0;
    while (str[len] && len < max)
    {
        len++;
    }

    return len;
}

char *NS(strcpy)(char *buf, const char *str)
{
    char *start = buf;
    while (*str != '\0')
    {
        *buf++ = *str++;
    }

    *buf = '\0';

    return start;
}

char *NS(strncpy)(char *buf, const char *str, size_t max)
{
    char *start = buf;
    while (*str != '\0' && max-- > 0)
    {
        *buf++ = *str++;
    }

    *buf = '\0';

    return start;
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int to_num_digit(char c)
{
    return c - 48;
}

int NS(strncmp)(const char *str1, const char *str2, size_t n)
{
    unsigned char u1, u2;

    while (n--)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;

        if (u1 != u2)
        {
            return u1 - u2;
        }
        if (u1 == '\0')
        {
            return 0;
        }
    }
    return 0;
}

int strnlen_terminator(const char *str, int max, char terminator)
{
    for (int i = 0; i < max; i++)
    {
        if (str[i] == '\0' || str[i] == terminator)
        {
            return i;
        }
    }
    return max;
}

char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + ('a' - 'A');
    }
    return c;
}

int NS(istrncmp)(const char *s1, const char *s2, size_t n)
{
    unsigned char u1, u2;

    while (n--)
    {
        u1 = (unsigned char)*s1++;
        u2 = (unsigned char)*s2++;

        if (u1 != u2 && to_lower(u1) != to_lower(u2))
        {
            return u1 - u2;
        }
        if (u1 == '\0')
        {
            return 0;
        }
    }
    return 0;
}
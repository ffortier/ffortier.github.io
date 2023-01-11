#include "./string.h"

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }

    return len;
}

size_t strnlen(const char *str, size_t max)
{
    size_t len = 0;
    while (str[len] && len < max)
    {
        len++;
    }

    return len;
}

char *strcpy(char *buf, const char *str)
{
    char *start = buf;
    while (*str != '\0')
    {
        *buf++ = *str++;
    }

    *buf = '\0';

    return start;
}

char *strncpy(char *buf, const char *str, size_t max)
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

int strncmp(const char *str1, const char *str2, size_t n)
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

int istrncmp(const char *s1, const char *s2, size_t n)
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

char *sp = NULL;
char *strtok(char *str, const char *delimiters)
{
    int i = 0;
    int len = strlen(delimiters);

    if (!str && !sp)
    {
        return 0;
    }

    if (str && !sp)
    {
        sp = str;
    }

    char *p_start = sp;

    while (1)
    {
        for (i = 0; i < len; i++)
        {
            if (*p_start == delimiters[i])
            {
                p_start++;
                break;
            }
        }

        if (i == len)
        {
            sp = p_start;
            break;
        }
    }

    if (*sp == '\0')
    {
        sp = 0;
        return sp;
    }

    while (*sp != '\0')
    {
        for (i = 0; i < len; i++)
        {
            if (*sp == delimiters[i])
            {
                *sp = '\0';
                break;
            }
        }

        sp++;

        if (i < len)
        {
            break;
        }
    }

    return p_start;
}
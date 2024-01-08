#include "io.h"
#include "status.h"
#include "kernel.h"
#include "memory/memory.h"
#include <stdarg.h>

#define TERMINAL_BUFFER_SIZE 256

typedef void(put_char)(void *, char);

struct terminal_state
{
    char buf[TERMINAL_BUFFER_SIZE];
    int index;
};

struct buffer_state
{
    char *buf;
    int index;
};

static void terminal_flush(struct terminal_state *state)
{
    print(state->buf);
    memset(state, 0, sizeof(struct terminal_state));
}

static void put_char_buffer(void *state, char ch)
{
    struct buffer_state *buffer_state = (struct buffer_state *)state;
    buffer_state->buf[buffer_state->index++] = ch;
}

static void put_char_terminal(void *state, char ch)
{
    struct terminal_state *terminal_state = (struct terminal_state *)state;
    terminal_state->buf[terminal_state->index++] = ch;

    if (terminal_state->index == TERMINAL_BUFFER_SIZE)
    {
        terminal_flush(terminal_state);
    }
}

static void do_printf(void *state, put_char *fn, const char *format, va_list argp)
{
    int i = -1;
    char *ss = 0;
    int j;

    while (format[++i])
    {
        switch (format[i])
        {
        case '%':
            switch (format[++i])
            {
            case 's':
                ss = va_arg(argp, char *);
                j = -1;

                while (ss[++j])
                {
                    (*fn)(state, ss[j]);
                }

                break;

            default:
                break;
            }
            break;

        default:
            (*fn)(state, format[i]);
            break;
        }
    }
}

int sprintf(char *str, const char *format, ...)
{
    va_list argp;
    va_start(argp, format);

    struct buffer_state buffer_state;

    memset(&buffer_state, 0, sizeof(buffer_state));

    buffer_state.buf = str;
    buffer_state.index = 0;

    do_printf(&buffer_state, &put_char_buffer, format, argp);

    buffer_state.buf[buffer_state.index] = '\0';

    return buffer_state.index;
}

void printf(char *str, const char *format, ...)
{
    va_list argp;
    va_start(argp, format);

    struct terminal_state terminal_state;

    memset(&terminal_state, 0, sizeof(terminal_state));
    do_printf(&terminal_state, &put_char_terminal, format, argp);
    terminal_flush(&terminal_state);
}
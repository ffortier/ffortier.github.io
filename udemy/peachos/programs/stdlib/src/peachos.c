#include "peachos.h"
#include "string.h"
#include "status.h"

void peachos_command_argument_free(struct command_argument *command_argument)
{
    if (command_argument)
    {
        peachos_command_argument_free(command_argument->next);
        peachos_free(command_argument);
    }
}

struct command_argument *peachos_parse_command(const char *command, int max)
{
    int res = 0;

    CHECK_ARG(max <= 1024);

    struct command_argument *root_command = 0;
    char scommand[1024];
    strncpy(scommand, command, sizeof(scommand));
    char *token = strtok(scommand, " ");

    if (!token)
    {
        goto out;
    }

    root_command = peachos_malloc(sizeof(struct command_argument));
    CHECK(root_command, -ENOMEM);
    strncpy(root_command->argument, token, sizeof(root_command->argument));
    struct command_argument *current = root_command;
    token = strtok(scommand, " ");

    while (token)
    {
        struct command_argument *next = peachos_malloc(sizeof(struct command_argument));
        CHECK(next, -ENOMEM);
        strncpy(next->argument, token, sizeof(next->argument));
        current->next = next;
        current = next;
        token = strtok(NULL, " ");
    }

out:
    if (res < 0)
    {
        peachos_command_argument_free(root_command);
        root_command = 0;
    }

    return root_command;
}

int peachos_getkeyblock()
{
    int val;

    do
    {
        val = peachos_getkey();
    } while (!val);

    return val;
}

void peachos_terminal_readline(char *out, int max, bool output_while_typing)
{
    for (int i = 0; i < max - 1; i++)
    {
        char c = peachos_getkeyblock();

        if (c == 13)
        {
            break;
        }

        if (output_while_typing)
        {
            peachos_putchar(c);
        }

        if (c == 8 && i > 0)
        {
            *--out = 0;
            i -= 2;
            continue;
        }

        *out++ = c;
    }

    *out = 0;
}

int peachos_system_run(const char *command)
{
    char buf[1024];
    strncpy(buf, command, sizeof(buf));
    struct command_argument *root = peachos_parse_command(buf, 1024);

    if (!root)
    {
        return -1;
    }

    return peachos_system(root);
}
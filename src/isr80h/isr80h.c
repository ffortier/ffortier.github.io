#include "isr80h.h"
#include "idt/idt.h"
#include "isr80h/misc.h"

void isr80h_register_commands()
{
    isr80h_register_command(SYSTEM_COMMAND_SUM, &isr80h_command0_sum);
    isr80h_register_command(SYSTEM_COMMAND_PRINT, &isr80h_command1_print);
    isr80h_register_command(SYSTEM_COMMAND_GETKEY, &isr80h_command2_getkey);
}
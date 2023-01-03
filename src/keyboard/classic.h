#ifndef KEYBOARD_CLASSIC_H
#define KEYBOARD_CLASSIC_H

struct keyboard *classic_init();

#define PS2_PORT 0x64
#define PS2_COMMAND_ENABLE_FIRST_PORT 0xae
#endif
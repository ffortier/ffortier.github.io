#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();
void print(const char *str);
void panic(const char *message);
void kernel_page();
void kernel_registers();
void terminal_writechar(char c, char colour);
void terminal_putchar(int x, int y, char c, char colour);

#endif
#include "common.h"

void vector2_debug(Vector2 vec, const char *preamble)
{
    printf("%s { .x = %.2f, .y = %.2f }\n", preamble, vec.x, vec.y);
}

void rect_debug(Rectangle rect, const char *preamble)
{
    printf("%s { .x = %.2f, .y = %.2f, .width = %.2f, .height = %.2f }\n", preamble, rect.x, rect.y, rect.width, rect.height);
}

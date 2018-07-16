#include "../include/text.h"
#include "../include/io.h"

utext_t* txt_get_ulines (const char* fname, int* num_lines)
{
    char** lines = io_get_lines(fname, num_lines);
    utext_t* ulines = (utext_t*) lines;

    return ulines;
}

void txt_print_uline (utext_t uline)
{
    printf("%s\n", (char*) uline);
}

void txt_print_ulines (utext_t* ulines, int num_lines)
{
    int i;

    for (i = 0; i < num_lines; i++)
    {
        txt_print_uline(ulines[i]);
    }
}
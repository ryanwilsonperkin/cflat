#include <stdarg.h>
#include <stdio.h>
#include "print.h"

#define INDENT_CHAR '-'
#define INDENT_SIZE 2

void print_at_depth
(FILE *out, int depth, const char *fmt, ...)
{
        int i, j;
        for (i = 0; i < depth; i++) {
                for (j = 0; j < INDENT_SIZE; j++) {
                        fprintf(out, "%c", INDENT_CHAR);
                }
        }
        va_list argptr;
        va_start(argptr, fmt);
        vfprintf(out, fmt, argptr);
        va_end(argptr);
        fprintf(out, "\n");
}

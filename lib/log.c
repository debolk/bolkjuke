#include "log.h"

#include <stdarg.h>
#include <stdio.h>

void
bolkjuke_log_init()
{
    freopen("bolkjuke.log", "a", stderr);
}

void
bolkjuke_log_destroy()
{
}

void
bolkjuke_log(const char *format, ...)
{
    va_list args;

    fprintf(stderr, "libbolkjuke: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

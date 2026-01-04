#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

static const char *level_str[] = {
    "DEBUG", "INFO", "WARNING", "ERROR"};

void log_message(log_level_t level, char *fmt, ...)
{
    // time_t now = time(NULL);
    // struct tm *local = localtime(&now);
    // char time_buf[20];
    // strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", local);
    fprintf(stderr, "[%s] ", level_str[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
}

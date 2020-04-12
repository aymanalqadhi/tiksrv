#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log/logger.h"

static log_level_t log_level;

static const char *level_names[] = { "FATAL", "ERROR", "WARN",
                                     "INFO",  "DEBUG", "TRACE" };

static const char *level_colors[] = { "\x1b[35m", "\x1b[31m", "\x1b[33m",
                                      "\x1b[32m", "\x1b[36m", "\x1b[94m" };
void
log_set_level(log_level_t level)
{
    log_level = level;
}

void
log_log(log_level_t level, const char *file, int line, const char *fmt, ...)
{
    time_t     t;
    va_list    args;
    char       buf[0x20];
    struct tm *lt;

    if (level < log_level) {
        return;
    }

    t  = time(NULL);
    lt = localtime(&t);

    buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
    fprintf(stderr,
            "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
            buf,
            level_colors[level],
            level_names[level],
            basename((char *)file),
            line);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

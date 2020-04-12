#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log/logger.h"

static struct
{
    void *udata;
    FILE *fp;
    int   level;
    int   quiet;
} L;

static const char *level_names[] = { "FATAL", "ERROR", "WARN",
                                     "INFO",  "DEBUG", "TRACE" };

#ifdef LOG_USE_COLOR
static const char *level_colors[] = { "\x1b[35m", "\x1b[31m", "\x1b[33m",
                                      "\x1b[32m", "\x1b[36m", "\x1b[94m" };
#endif

void
log_set_udata(void *udata)
{
    L.udata = udata;
}

void
log_set_fp(FILE *fp)
{
    L.fp = fp;
}

void
log_set_level(int level)
{
    L.level = level;
}

void
log_set_quiet(int enable)
{
    L.quiet = enable ? 1 : 0;
}

void
log_log(int level, const char *file, int line, const char *fmt, ...)
{
    time_t     t;
    va_list    args;
    char       buf[0x20];
    struct tm *lt;

    if (level < L.level) {
        return;
    }

    t  = time(NULL);
    lt = localtime(&t);

    if (!L.quiet) {
        buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
#ifdef LOG_USE_COLOR
        fprintf(stderr,
                "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
                buf,
                level_colors[level],
                level_names[level],
                basename((char *)file),
                line);
#else
        fprintf(stderr, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
#endif
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, "\n");
        fflush(stderr);
    }

    /* Log to file */
    if (L.fp) {
        buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
        fprintf(L.fp, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
        va_start(args, fmt);
        vfprintf(L.fp, fmt, args);
        va_end(args);
        fprintf(L.fp, "\n");
        fflush(L.fp);
    }
}

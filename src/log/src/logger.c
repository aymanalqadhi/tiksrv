#include "log/colors.h"
#include "log/logger.h"

#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const static char *level_labels[7] = {
    COLORED("fatal", COLOR_BRED),
    COLORED("error", COLOR_RED),
    COLORED("warn", COLOR_YELLOW),
    COLORED("info", COLOR_CYAN),
    COLORED("norm", COLOR_MAGENTA),
    COLORED("debug", COLOR_BLUE),
    "trace",
};

static log_level_t log_level;

void
log_set_level(log_level_t level)
{
    log_level = level;
}

void
log_log(log_level_t level, const char *file, int line, const char *fmt, ...)
{
    if (level == LOG_FATAL) {
        printf("Here i am\n");
    }
    time_t     current_time;
    va_list    vl;
    char       time_buf[0x20];
    FILE *     log_file;
    struct tm *tm;

    if (level > log_level) {
        return;
    }

    current_time = time(NULL);
    log_file     = level >= LOG_WARN ? stderr : stdout;

    if (!(tm = localtime(&current_time)) ||
        strftime(time_buf, sizeof(time_buf), "%H:%M:%S", tm) < 0) {
        strncpy(time_buf, "[00:00:00]", sizeof(time_buf));
    }

    fprintf(log_file,
            "[%s] [" COLORED("%s:%d", COLOR_GRAY) "] [%s] ",
            time_buf,
            basename((char *)file),
            line,
            level_labels[level]);

    va_start(vl, fmt);
    vfprintf(log_file, fmt, vl);
    va_end(vl);

    fputc('\n', log_file);
}

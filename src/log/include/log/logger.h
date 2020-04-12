#ifndef TIKSRV_LOG_LOGGER_H
#define TIKSRV_LOG_LOGGER_H

#include <stdarg.h>
#include <stdio.h>

#define LOG_USE_COLOR

enum
{
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_TRACE
};

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void
log_set_udata(void *udata);
void
log_set_fp(FILE *fp);
void
log_set_level(int level);
void
log_set_quiet(int enable);

void
log_log(int level, const char *file, int line, const char *fmt, ...);

#endif

#ifndef TIKSRV_LOG_LOGGER_H
#define TIKSRV_LOG_LOGGER_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum
{
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_NORM,
    LOG_DEBUG,
    LOG_TRACE
} log_level_t;

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/*!
 * \brief Sets the logger minimum logging level
 *
 * \param [in] level  The new minimum logging level
 */
void
log_set_level(log_level_t level);

/*!
 * \brief Logs a message to the console
 *
 * \param [in] level  The log message level
 * \param [in] file   A pointer to the file name from which the logging macro
 *                    was called
 * \param [in] line   The line number from which to the logging macro was called
 * \param [in] fmt    A the logging message format
 */
void
log_log(log_level_t level, const char *file, int line, const char *fmt, ...);

#endif

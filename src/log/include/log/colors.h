#ifndef TIKSRV_LOG_COLORS_H
#define TIKSRV_LOG_COLORS_H

#include <stdint.h>

#define COLOR_RESET "\033[0m"

#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_GRAY    "\033[90m"

#define COLOR_BRED     "\033[31;1m"
#define COLOR_BGREEN   "\033[32;1m"
#define COLOR_BYELLOW  "\033[33;1m"
#define COLOR_BBLUE    "\033[34;1m"
#define COLOR_BMAGENTA "\033[35;1m"
#define COLOR_BCYAN    "\033[36;1m"

#define COLORED(str, color) color str COLOR_RESET

#endif

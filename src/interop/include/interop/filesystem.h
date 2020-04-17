#ifndef TIKSRV_INTEROP_FILESYSTEM_H
#define TIKSRV_INTEROP_FILESYSTEM_H

#include "log/error.h"

#include "uv.h"

#include <stdbool.h>

typedef ts_error_t (*ts_dir_iteration_cb)(const uv_dirent_t *, void *ctx);

bool
ts_path_exists(const char *path);

bool
ts_is_file(const char *path);

bool
ts_is_directory(const char *path);

ts_error_t
ts_cwd(char *buf, size_t len);

ts_error_t
ts_chdir(const char *path);

ts_error_t
ts_chdir_cwd(const char *path, char *buf, size_t len);

ts_error_t
ts_iterate_directory(const char *        dirname,
                     bool                chdir,
                     ts_dir_iteration_cb cb,
                     void *              ctx);

#endif

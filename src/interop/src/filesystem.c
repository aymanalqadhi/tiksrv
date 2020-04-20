#include "interop/filesystem.h"
#include "log/logger.h"
#include "util/validation.h"

#include <uv.h>

#include <stdbool.h>
#include <sys/stat.h>

static inline bool
stat_path(const char *path, uv_fs_t *fs)
{
    int rc;

    if (!path || !fs) {
        return false;
    }

    if ((rc = uv_fs_stat(uv_default_loop(), fs, path, NULL)) < 0) {
        log_debug("uv_fs_stat: %s", uv_strerror(rc));
        return false;
    }

    return true;
}

static inline bool
check_stat_mode_flag(const char *path, int flag)
{
    uv_fs_t req;
    bool    ret;

    if (!stat_path(path, &req)) {
        uv_fs_req_cleanup(&req);
        return false;
    }

    ret = (req.statbuf.st_mode & flag) != 0;
    uv_fs_req_cleanup(&req);

    return ret;
}

bool
ts_path_exists(const char *path)
{
    uv_fs_t req;
    bool    ret;

    ret = stat_path(path, &req);
    uv_fs_req_cleanup(&req);

    return ret;
}

bool
ts_is_file(const char *path)
{
    return check_stat_mode_flag(path, S_IFREG);
}

bool
ts_is_directory(const char *path)
{
    return check_stat_mode_flag(path, S_IFDIR);
}

ts_error_t
ts_cwd(char *buf, size_t len)
{
    int rc;

    CHECK_NULL_PARAMS_1(buf);

    if ((rc = uv_cwd(buf, &len)) < 0) {
        log_error("uv_cwd: %s", uv_strerror(rc));
        return TS_ERR_GET_CWD_FAILED;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_chdir(const char *path)
{
    int rc;

    CHECK_NULL_PARAMS_1(path);

    if ((rc = uv_chdir(path)) < 0) {
        log_error("uv_chdir: %s", uv_strerror(rc));
        return TS_ERR_CHDIR_FAILED;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_chdir_cwd(const char *path, char *buf, size_t len)
{
    int rc;

    if ((rc = ts_cwd(buf, len)) != 0 || (rc = ts_chdir(path)) != 0) {
        return rc;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_iterate_directory(const char *        dirname,
                     bool                chdir,
                     ts_dir_iteration_cb cb,
                     void *              ctx)
{
    int         rc;
    char        cwd_buf[FILENAME_MAX];
    uv_fs_t     req;
    uv_dirent_t ent;

    CHECK_NULL_PARAMS_2(dirname, cb);

    if (!ts_is_directory(dirname)) {
        return TS_ERR_SCAN_DIR_FAILED;
    }

    if ((rc = uv_fs_scandir(uv_default_loop(), &req, dirname, 0, NULL) < 0)) {
        log_error("uv_fs_scandir: %s", uv_strerror(rc));
        return TS_ERR_SCAN_DIR_FAILED;
    }

    if (chdir && (rc = ts_chdir_cwd(dirname, cwd_buf, sizeof(cwd_buf)))) {
        goto cleanup;
    }

    while (uv_fs_scandir_next(&req, &ent) != UV_EOF) {
        if ((rc = (*cb)(&ent, ctx)) != 0) {
            goto cleanup;
        }
    }

    if (chdir && (rc = ts_chdir(cwd_buf)) != 0) {
        goto cleanup;
    }

    rc = TS_ERR_SUCCESS;

cleanup:
    uv_fs_req_cleanup(&req);
    return rc;
}

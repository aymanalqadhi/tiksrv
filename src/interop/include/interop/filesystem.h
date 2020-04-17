#ifndef TIKSRV_INTEROP_FILESYSTEM_H
#define TIKSRV_INTEROP_FILESYSTEM_H

#include "log/error.h"

#include "uv.h"

#include <stdbool.h>

typedef ts_error_t (*ts_dir_iteration_cb)(const uv_dirent_t *, void *ctx);

/*!
 * \biref Checks whether a path pointed to by \see path exists on the filesystem
 *        or not
 *
 * \param [in] path  A pointer to the path string to be checked
 *
 * \return True if the path exists, false otherwise
 */
bool
ts_path_exists(const char *path);

/*!
 * \biref Checks if a path pointed to by \see path points to a file on the
 *        filesystem
 *
 * \param [in] path  A pointer to the path string to be checked
 *
 * \return True if the path poins to a file, false otherwise
 */
bool
ts_is_file(const char *path);

/*!
 * \biref Checks if a path pointed to by \see path points to a directory on the
 *        filesystem
 *
 * \param [in] path  A pointer to the path string to be checked
 *
 * \return True if the path poins to a directory, false otherwise
 */
bool
ts_is_directory(const char *path);

/*!
 * \brief Gets the current working directory and save to a buffer pointer to by
 *        \see buf of length \see len
 *
 * \param [out] buf  A pointer to output buffer
 * \param [in]  len  The length of the passed buffer
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_cwd(char *buf, size_t len);

/*!
 * \brief Changes the current working directory to a path pointed to by \see
 *        path
 *
 * \param [in] path  A pointer to the new working directory path
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_chdir(const char *path);

/*!
 * \brief Changes the current working directory to a path pointed to by \see
 *        path, and save the previous working directory to a buffer pointed to
 *        by \see buf of size \see len
 *
 * \param [in]  path  A pointer to the new current working directory path
 * \param [out] buf   A pointer tot into which to save the previous working
 *                    directory
 * \param [in]  len   The length of the previous working directory output buffer
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_chdir_cwd(const char *path, char *buf, size_t len);

/*!
 * \brief Iterates a directory of name \see dirname, and apply a callback on
 *        each entry in that directory
 *
 * This function iterates all entries of a directory of the name pointed to by
 * \see dirname. If \see chdir is true, then this function will change the
 * current working directory into that directory, leaving it afterwards.
 * The callback function pointed to \see cb is applied to each entry in that
 * directory with the context pointer \see ctx passwd to the callback function.
 * If any iteratetion returns a non-success value (0), the iteration is stopped
 * and the returned value is then returned.
 *
 * \param [in] dirname  A pointer to the directory name which to be iterateted
 * \param [in] chdir    If true, the current working directory is changed to
 *                      \see dirname
 * \param [in] cb       A callback function pointer to be appliced on each
 *                      directory entry
 * \param [in] ctx      A context pointer to be passed to the callback function
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_iterate_directory(const char *        dirname,
                     bool                chdir,
                     ts_dir_iteration_cb cb,
                     void *              ctx);

#endif

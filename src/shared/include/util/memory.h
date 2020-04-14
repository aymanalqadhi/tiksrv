#ifndef TIKSRV_SHARED_UTIL_MEMORY_H
#define TIKSRV_SHARED_UTIL_MEMORY_H

#include <stddef.h>

typedef void (*ezd_dealloc_func_t)(void *);
typedef int (*memory_compare_func_t)(const void *, const void *, const void *);

/*!
 * \brief Duplicates a memory buffer
 *
 * This function has a behavior similar to \see strdup, but works for generic
 * memory buffers.
 *
 * \param [in] mem  A pointer to the buffer to be duplicted
 * \param [in] len  The number of bytes to duplicate
 *
 * \return A pointer to the duplicated memory buffer, or NULL on failure
 */
void *
ts_memdup(const void *mem, size_t len);

/*!
 * \biref Swaps \see len bytes of two memory buffers data
 *
 * \param [in, out] a    A pointer to the first memory buffer
 * \param [in, out] b    A pointer to the second memory buffer
 * \param [in]      len  The amount of bytes to swap
 *
 * \return 0 on success, or a negative value indicating error on failure
 */
int
ts_memxchg(void *a, void *b, size_t len);

#endif

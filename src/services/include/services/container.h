#ifndef TIKSRV_SERVICES_CONTAINER_H
#define TIKSRV_SERVICES_CONTAINER_H

#include "services/hooks_manager.h"

#include "log/error.h"

struct ts_services_container
{
    struct ts_hooks_manager *hooks_manager;
};

/*!
 * \brief Creates a new service container
 *
 * \return The created container
 */
struct ts_services_container *
ts_services_container_new(void);

/*!
 * \brief Initializes a service container pointed to by \see container services
 *
 * \param [in, out] container  A pointer to the services container which to be
 *                             initialized
 *
 * \return 0 on success, or a negative value on failrue
 */
ts_error_t
ts_services_container_init(struct ts_services_container *container);

/*!
 * \brief Frres a services container pointed to by \see container
 *        deinitializing its services
 *
 * \param [in, out] container  A pointer to the services container which to be
 *                             freed
 */
void
ts_services_container_free(struct ts_services_container *container);

#endif

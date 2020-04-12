#ifndef TIKSRV_APP_APP_H
#define TIKSRV_APP_APP_H

#include "config/config.h"
#include "log/error.h"

/*!
 * \brief Runs the application
 *
 * This function is the entry point of the application as whole. This function
 * initializes and starts all other application componenents.
 *
 * \param [in] cfg  A pointer to the application configuration object
 *
 * \return 0 on success, or a negative vaue indicating error otheriwse
 */
ts_error_t
ts_app_run(const struct ts_config *cfg);

/*!
 * \brief Free all resources held by the application
 *
 * This function prepares the application to be shutdown freeing all used
 * resources by the application
 */
void
ts_app_destroy(void);

#endif

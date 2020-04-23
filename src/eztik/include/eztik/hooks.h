#ifndef EZTIK_HOOKS_H
#define EZTIK_HOOKS_H

/*!
 * \brief A hook callback function to be called when a new client is connected
 *
 * \param [in] idptr  A pointer to the new client id
 * \param [in] ctx    A pointer to context data set by the plugin
 */
void
eztik_connection_hook(const void *idptr, void *ctx);

/*!
 * \brief A hook callback function to be called when a client is disconnected
 *
 * \param [in] idptr  A pointer to the disconnected client id
 * \param [in] ctx    A pointer to context data set by the plugin
 */
void
eztik_disconnection_hook(const void *idptr, void *ctx);

#endif

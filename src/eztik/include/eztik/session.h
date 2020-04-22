#ifndef EZTIK_SESSION_H
#define EZTIK_SESSION_H

#include "eztik/routeros/api.h"

struct eztik_session
{
    uint32_t       id;
    struct ros_api api;
};

/*!
 * \brief Creates a new session with an id of \see id
 *
 * \param [in] id  The id to use for the created session
 *
 * \return A pointer to the created session object
 */
struct eztik_session *
eztik_session_new(uint32_t id);

/*!
 * \brief Closes a session pointed to by \see session
 *
 * \param [in, out] session  A pointer to the session to be closed
 */
void
eztik_session_close(struct eztik_session *session);

/*!
 * \brief Frees the resources used by  a session pointed to by \see session
 *
 * \param [in, out] session  A pointer to the session to be freed
 */
void
eztik_session_free(struct eztik_session *session);

#endif

#ifndef TIKSRV_ERROR_H
#define TIKSRV_ERROR_H

typedef enum
{
    TS_ERR_SUCCESS = 0,

    /* First error marer (used by ezd_strerror) */
    TS_ERR_FIRST_MARKER = -0x1000,

    /* General Errors */
    TS_ERR_NULL_ARGUMENTS,
    TS_ERR_UNINITIALIZED_DATA,
    TS_ERR_MEMORY_ALLOC_FAILED,
    TS_ERR_NOT_FOUND,
    TS_ERR_INDEX_OUT_OF_RANGE,
    TS_ERR_NOT_IMPLEMENTED,

    /* Network errors */
    TS_ERR_SOCKET_CREATE_FAILED,
    TS_ERR_SOCKET_CLOSE_FAILED,
    TS_ERR_LISTENER_BIND_FAILED,
    TS_ERR_LISTENER_STOP_FAILED,
    TS_ERR_EVENT_LOOP_START_FAILED,
    TS_ERR_LISTENER_ALREADY_STARTED,
    TS_ERR_LOAD_ADDRESS_FAILED,

    /* Last error marer (used by ezd_strerror) */
    TS_ERR_LAST_MARKER
} ts_error_t;

const char *
ts_strerror(int err);

#endif

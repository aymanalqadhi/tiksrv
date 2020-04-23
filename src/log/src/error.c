#include "log/error.h"

#include <errno.h>
#include <string.h>

static const char *error_messages[] = {
    "NULL arguments were passed to the function",
    "An attempt to use uninitialized data",
    "No matching items were found",
    "Index out of range",
    "Not implemented",
    "Could not create socket",
    "Could not open socket",
    "Could not close socket",
    "Could not write to socket",
    "The socket is in an invalid state",
    "Could not bind listener",
    "Could not stop listener",
    "The listener is already started",
    "Could not start event loop",
    "Could not get the address of the specified socket",
    "Could not encode response message header",
    "Could not export commands",
    "Could not load plugin",
    "An attempt to load an invalid plugin",
    "Could not iterate directory entries",
    "Could not get current working directory",
    "Could not change directory",
    "Could not parse configuration file",
    "A required configuration value was not found in the configuration file"
};

const char *
ts_strerror(ts_error_t err)
{
    if (err > TS_ERR_FIRST_MARKER && err < TS_ERR_LAST_MARKER) {
        return error_messages[err + -TS_ERR_FIRST_MARKER - 2];
    }

    return strerror(err);
}

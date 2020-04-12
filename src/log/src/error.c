#include "log/error.h"

#include <errno.h>
#include <string.h>

static const char *error_messages[] = {
    "NULL arguments were passed to the function",
    "An attempt to use uninitialized data",
    "Could not allocate memory",
    "No matching items were found",
    "Index out of range",
    "Not implemented",
    "Could not open socket",
    "Could not close socket",
    "Could not bind listener",
    "Could not stop listener",
    "Could not start event loop",
    "The listener is already started",
    "Could not get the address of the specified socket"
};

const char *
ts_strerror(int err)
{
    if (err > TS_ERR_FIRST_MARKER && err < TS_ERR_LAST_MARKER) {
        return error_messages[err + -TS_ERR_FIRST_MARKER - 1];
    }

    return strerror(err);
}

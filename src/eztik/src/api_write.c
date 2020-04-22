#include "eztik/routeros/api.h"

#include "log/error.h"

ts_error_t
ros_api_write(struct ros_api *           api,
              const struct ros_sentence *sentence,
              ros_api_write_cb           cb,
              void *                     ctx)
{
    return TS_ERR_SUCCESS;
}

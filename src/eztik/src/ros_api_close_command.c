#include "eztik/commands.h"
#include "eztik/eztik.h"
#include "eztik/response.h"
#include "eztik/session.h"

#include "commands/command.h"
#include "commands/request.h"
#include "commands/response.h"
#include "log/error.h"
#include "log/logger.h"

static void
ros_api_close_command_exec(const struct ts_request *req)
{
    struct ts_response *resp;

    resp = ts_respone_new(req);

    ts_response_write_string(resp, "Dummy Close");
    ts_response_commit(resp);

    ts_response_free(resp);
}

struct ts_command eztik_ros_api_close_command = {
    .command = EZTIK_COMMAND_ROS_API_CLOSE,
    .func    = &ros_api_close_command_exec
};

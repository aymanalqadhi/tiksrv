#include "eztik/commands.h"

#include "commands/command.h"
#include "commands/request.h"
#include "commands/response.h"

static void
ros_version_command_exec(const struct ts_request *req)
{
    struct ts_response *resp = ts_respone_new(req);

    ts_response_write_string(resp, "Dummy Version");
    ts_response_commit(resp);
    ts_response_free(resp);
}

struct ts_command eztik_ros_version_command = {
    .command = EZTIK_COMMAND_ROS_VERSION,
    .func    = &ros_version_command_exec
};

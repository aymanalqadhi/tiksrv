#ifndef EZTIK_COMMANDS_H
#define EZTIK_COMMANDS_H

#include "commands/command.h"

#define EZTIK_COMMANDS_TYPE 0x1000

enum
{
    EZTIK_COMMAND_ROS_API_OPEN = 0x1000,
    EZTIK_COMMAND_ROS_API_CLOSE,
    EZTIK_COMMAND_ROS_VERSION
};

extern struct ts_command eztik_ros_api_open_command;
extern struct ts_command eztik_ros_api_close_command;
extern struct ts_command eztik_ros_version_command;

#endif

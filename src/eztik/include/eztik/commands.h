#ifndef EZTIK_COMMANDS_H
#define EZTIK_COMMANDS_H

#include "commands/command.h"

#define EZTIK_COMMANDS_TYPE 0x1000

enum
{
    EZTIK_COMMAND_ROS_API_OPEN     = 0x1000,
    EZTIK_COMMAND_ROS_API_CLOSE    = 0x1001,
    EZTIK_COMMAND_ROS_API_SEND_RAW = 0x1002,
    EZTIK_COMMAND_ROS_VERSION      = 0x1003
};

extern struct ts_command eztik_ros_api_open_command;
extern struct ts_command eztik_ros_api_close_command;
extern struct ts_command eztik_ros_api_send_raw_command;
extern struct ts_command eztik_ros_version_command;

#endif

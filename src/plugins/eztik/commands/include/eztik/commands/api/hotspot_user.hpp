#ifndef EZTIK_COMMANDS_API_HOTSPOT_USER_HPP
#define EZTIK_COMMANDS_API_HOTSPOT_USER_HPP

#include "eztik/commands/api/load.hpp"
#include "eztik/data/models/hotspot_user.hpp"

namespace eztik::commands::api {

namespace detail {

using eztik::data::models::hotspot_user;

class hotspot_user_load_command final : public load_command<hotspot_user> {
  public:
    hotspot_user_load_command(ts::log::logger &                  logger,
                              eztik::services::sessions_service &sessions_svc)
        : load_command<hotspot_user> {"Hotspot Users Load", logger,
                                      sessions_svc} {
    }
};

} // namespace detail

using detail::hotspot_user_load_command;

} // namespace eztik::commands::api

#endif

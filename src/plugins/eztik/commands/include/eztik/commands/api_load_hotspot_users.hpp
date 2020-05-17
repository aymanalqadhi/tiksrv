#ifndef EZTIK_COMMANDS_API_LOAD_HOTSPOT_USERS_HPP
#define EZTIK_COMMANDS_API_LOAD_HOTSPOT_USERS_HPP

#include "eztik/commands/session_required.hpp"
#include "eztik/services/sessions.hpp"

#include "log/logger.hpp"

#include <string>

namespace eztik::commands {

class api_load_hotspot_users_command final : public session_required_command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    api_load_hotspot_users_command(
        ts::log::logger &                  logger,
        eztik::services::sessions_service &sessions_svc)
        : session_required_command {"RouterOS Load Hotspot Users", logger,
                                    sessions_svc} {
    }

    void execute(client_ptr client, ts::net::request &&req) override;
};

} // namespace eztik::commands

#endif

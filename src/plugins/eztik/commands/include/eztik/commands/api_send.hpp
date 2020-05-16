#ifndef EZTIK_COMMANDS_API_SEND_HPP
#define EZTIK_COMMANDS_API_SEND_HPP

#include "eztik/commands/session_required.hpp"
#include "eztik/services/sessions.hpp"

#include "log/logger.hpp"

#include <string>

namespace eztik::commands {

class api_send_command final : public session_required_command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    api_send_command(ts::log::logger &                  logger,
                     eztik::services::sessions_service &sessions_svc)
        : session_required_command {"RouterOS API Send", logger, sessions_svc} {
    }

    void execute(client_ptr client, ts::net::request &&req) override;
};

} // namespace eztik::commands

#endif

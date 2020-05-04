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
        : name_ {"RouterOS API Send"},
          session_required_command {logger, sessions_svc} {
    }

    auto name() const noexcept -> const std::string & override {
        return name_;
    }

    void execute(client_ptr client, ts::net::request &&req) override;

  private:
    std::string name_;
};

} // namespace eztik::commands

#endif

#ifndef EZTIK_COMMANDS_API_SEND_HPP
#define EZTIK_COMMANDS_API_SEND_HPP

#include "eztik/services/sessions.hpp"

#include "interop/command.hpp"
#include "log/logger.hpp"

#include <string>

namespace eztik::commands {

class api_send_command final : public ts::interop::command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    api_send_command(ts::log::logger &                  logger,
                     eztik::services::sessions_service &sessions_svc)
        : name_ {"RouterOS API Send"},
          logger_ {logger},
          sessions_svc_ {sessions_svc} {
    }

    auto name() const noexcept -> const std::string & override {
        return name_;
    }

    void execute(client_ptr client, ts::net::request &&req) override;

  private:
    std::string name_;

    ts::log::logger &                  logger_;
    eztik::services::sessions_service &sessions_svc_;
};

} // namespace eztik::commands

#endif

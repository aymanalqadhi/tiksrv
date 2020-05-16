#ifndef EZTIK_COMMANDS_SESSION_HPP
#define EZTIK_COMMANDS_SESSION_HPP

#include "eztik/services/sessions.hpp"

#include "interop/command.hpp"
#include "log/logger.hpp"

#include <memory>

namespace eztik::commands {

class session_required_command : public ts::interop::command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    session_required_command(const std::string &                name,
                             ts::log::logger &                  logger,
                             eztik::services::sessions_service &sessions_svc)
        : name_ {name},
          logger_ {logger},
          sessions_svc_ {sessions_svc},
          session_ {nullptr} {
    }

    auto name() const noexcept -> const std::string & final {
        return name_;
    }

    void operator()(client_ptr client, ts::net::request &&req) override;

  protected:
    std::string                               name_;
    ts::log::logger &                         logger_;
    eztik::services::sessions_service &       sessions_svc_;
    std::shared_ptr<eztik::services::session> session_;
};

} // namespace eztik::commands

#endif

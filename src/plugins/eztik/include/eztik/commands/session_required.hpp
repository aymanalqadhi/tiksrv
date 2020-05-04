#ifndef EZTIK_COMMANDS_SESSION_HPP
#define EZTIK_COMMANDS_SESSION_HPP

#include "eztik/routeros/sentence.hpp"
#include "eztik/services/sessions.hpp"

#include "interop/command.hpp"
#include "log/logger.hpp"

#include <memory>

namespace eztik::commands {

class session_required_command : public ts::interop::command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    session_required_command(ts::log::logger &                  logger,
                             eztik::services::sessions_service &sessions_svc)
        : logger_ {logger}, sessions_svc_ {sessions_svc}, session_ {nullptr} {
    }

    void operator()(client_ptr client, ts::net::request &&req) override;

  protected:
    ts::log::logger &                  logger_;
    eztik::services::sessions_service &sessions_svc_;
    std::shared_ptr<eztik::session>    session_;
};

} // namespace eztik::commands

#endif

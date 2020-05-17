#ifndef EZTIK_COMMANDS_SESSION_HPP
#define EZTIK_COMMANDS_SESSION_HPP

#include "eztik/services/sessions.hpp"

#include "interop/command.hpp"
#include "log/logger.hpp"

#include <memory>
#include <utility>

namespace eztik::commands {

class session_required_command : public ts::interop::command {
  public:
    session_required_command(
        std::string &&                           name,
        ts::log::logger &                        logger,
        eztik::services::sessions_service &sessions_svc)
        : ts::interop::command {std::move(name)},
          logger_ {logger},
          sessions_svc_ {sessions_svc},
          session_ {nullptr} {
    }

    void operator()(std::shared_ptr<ts::net::tcp_client> client,
                    ts::net::request &&                  req) override;

    [[nodiscard]] inline auto logger() noexcept -> ts::log::logger & {
        return logger_;
    }

    [[nodiscard]] inline auto sessions_service() const noexcept
        -> const eztik::services::sessions_service & {
        return sessions_svc_;
    }

    [[nodiscard]] inline auto current_session() noexcept
        -> std::shared_ptr<eztik::services::session> & {
        return session_;
    }

  private:
    ts::log::logger &                         logger_;
    eztik::services::sessions_service & sessions_svc_;
    std::shared_ptr<eztik::services::session> session_;
};

} // namespace eztik::commands

#endif

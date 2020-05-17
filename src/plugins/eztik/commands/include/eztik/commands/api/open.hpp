#ifndef EZTIK_COMMANDS_API_OPEN_HPP
#define EZTIK_COMMANDS_API_OPEN_HPP

#include "eztik/services/sessions.hpp"

#include "interop/command.hpp"
#include "log/logger.hpp"

#include <string>

namespace eztik::commands::api {

class open_command final : public ts::interop::command {
  public:
    open_command(ts::log::logger &                        logger,
                 eztik::services::sessions_service &sessions_svc)
        : ts::interop::command {"RouterOS API Open"},
          logger_ {logger},
          sessions_svc_ {sessions_svc} {
    }

    void execute(std::shared_ptr<ts::net::tcp_client> client,
                 ts::net::request &&                  req) override;

  private:
    ts::log::logger &                        logger_;
    eztik::services::sessions_service &sessions_svc_;
};

} // namespace eztik::commands::api

#endif

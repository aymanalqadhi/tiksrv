#ifndef EZTIK_SERVICES_SESSIONS_HPP
#define EZTIK_SERVICES_SESSIONS_HPP

#include "eztik/routeros/api.hpp"
#include "eztik/session.hpp"

#include "config/config.hpp"
#include "log/logger.hpp"
#include "services/hooks_manager.hpp"

#include <boost/asio/io_context.hpp>

#include <cstdint>
#include <map>
#include <memory>

namespace eztik::services {

class session_handler;

class sessions_service final : public eztik::session_handler {
  public:
    sessions_service(const ts::config::config &                   conf,
                     boost::asio::io_context &                    io,
                     ts::log::logger &                            logger,
                     std::shared_ptr<ts::services::hooks_manager> hooks_manager)
        : conf_ {conf},
          io_ {io},
          logger_ {logger},
          hooks_manager_ {std::move(hooks_manager)} {
        setup_hooks();
    }

    inline auto has(std::uint32_t id) const noexcept -> bool {
        return sessions_.contains(id);
    }

    inline auto operator[](std::uint32_t id) noexcept
        -> std::shared_ptr<session> {
        return sessions_[id];
    }

    void create(std::uint32_t                                 id,
                std::function<void(std::shared_ptr<session>)> cb);

    void close(std::uint32_t id);

  public:
    void on_close(const eztik::session& s) override;

  private:
    void setup_hooks();

  private:
    const ts::config::config &                   conf_;
    boost::asio::io_context &                    io_;
    ts::log::logger &                            logger_;
    std::shared_ptr<ts::services::hooks_manager> hooks_manager_;

    std::map<std::uint32_t, std::shared_ptr<eztik::session>> sessions_;
};

} // namespace eztik::services

#endif

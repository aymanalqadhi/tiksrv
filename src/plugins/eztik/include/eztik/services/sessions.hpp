#ifndef EZTIK_SERVICES_SESSIONS_HPP
#define EZTIK_SERVICES_SESSIONS_HPP

#include "eztik/routeros/api.hpp"

#include "config/config.hpp"
#include "log/logger.hpp"
#include "services/hooks_manager.hpp"

#include <boost/asio/io_context.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>

namespace eztik::services {

class session final {
  public:
    explicit session(eztik::routeros::api &&api) : api_ {std::move(api)} {
        assert(api_.is_open());
    }

    session(session &&rh) : id_ {rh.id_}, api_ {std::move(rh.api_)} {
    }

    session(boost::asio::io_context &io) : api_ {io} {
    }

    inline auto id() const noexcept -> std::uint32_t {
        return id_;
    }

    inline auto api() noexcept -> eztik::routeros::api & {
        return api_;
    }

  private:
    std::uint32_t        id_;
    eztik::routeros::api api_;
};

class sessions_service final {
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

  private:
    void setup_hooks();

  private:
    const ts::config::config &                   conf_;
    boost::asio::io_context &                    io_;
    ts::log::logger &                            logger_;
    std::shared_ptr<ts::services::hooks_manager> hooks_manager_;

    std::unordered_map<std::uint32_t, std::shared_ptr<session>> sessions_;
};

} // namespace eztik::services

#endif

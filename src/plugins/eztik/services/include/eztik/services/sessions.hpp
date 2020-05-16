#ifndef EZTIK_SERVICES_SESSIONS_HPP
#define EZTIK_SERVICES_SESSIONS_HPP

#include "eztik/routeros/api.hpp"

#include "config/config.hpp"
#include "log/logger.hpp"
#include "services/hooks_manager.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <cstdint>
#include <functional>
#include <map>
#include <memory>

namespace eztik::services {

class session;
class session_handler;

class session_handler {
  public:
    virtual void on_close(const session &s) = 0;
};

class session final : public eztik::routeros::api_handler {
  public:
    session(std::uint32_t            id,
            boost::asio::io_context &io,
            ts::log::logger &        logger,
            session_handler &        handler)
        : id_ {id},
          logger_ {logger},
          handler_ {handler},
          api_ {eztik::routeros::api::create(io, *this)} {
    }

    ~session() {
        if (api_->is_open()) {
            api_->close();
        }
    }

    inline auto id() const noexcept -> const std::uint32_t & {
        return id_;
    }

    inline auto api() const noexcept
        -> const std::shared_ptr<eztik::routeros::api> & {
        return api_;
    }

    inline auto is_ready() const noexcept -> bool {
        return api_->is_open() && api_->is_logged_in();
    }

  public:
    void on_error(const boost::system::error_code &err) override;

  private:
    std::uint32_t    id_;
    ts::log::logger &logger_;
    session_handler &handler_;

    std::shared_ptr<eztik::routeros::api> api_;
};

class sessions_service final : public session_handler {
    using session_open_cb  = std::function<void(
        const boost::system::error_code &, std::shared_ptr<session>)>;
    using session_close_cb = std::function<void(std::shared_ptr<session>)>;

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

    inline auto operator[](std::uint32_t id) noexcept
        -> std::shared_ptr<session> {
        return sessions_[id].first;
    }

    inline auto has(std::uint32_t id) const noexcept -> bool {
        return sessions_.contains(id);
    }

    inline auto has_ready(std::uint32_t id) const noexcept -> bool {
        return has(id) && sessions_.at(id).first->is_ready();
    }

    void create(std::uint32_t    id,
                session_open_cb  open_cb,
                session_close_cb close_cb);

    void close(std::uint32_t id);

  public:
    void on_close(const session &s) override;

  private:
    void setup_hooks();

  private:
    const ts::config::config &                   conf_;
    boost::asio::io_context &                    io_;
    ts::log::logger &                            logger_;
    std::shared_ptr<ts::services::hooks_manager> hooks_manager_;

    std::map<std::uint32_t,
             std::pair<std::shared_ptr<session>, session_close_cb>>
        sessions_;
};

} // namespace eztik::services

#endif

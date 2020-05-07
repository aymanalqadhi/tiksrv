#ifndef EZTIK_SESSION_HPP
#define EZTIK_SESSION_HPP

#include "routeros/api.hpp"
#include "routeros/sentence.hpp"

#include "log/logger.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <cstdint>

namespace eztik {

class session;

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
          api_ {id, io, logger, *this},
          logger_ {logger},
          handler_ {handler} {
    }

    ~session() {
        api_.close();
    }

    inline auto id() const noexcept -> const std::uint32_t & {
        return id_;
    }

    inline auto api() noexcept -> eztik::routeros::api & {
        return api_;
    }

    inline auto is_ready() const noexcept -> bool {
        return api_.is_ready();
    }

  public:
    void on_error(const boost::system::error_code &err) override;

  private:
    std::uint32_t        id_;
    eztik::routeros::api api_;
    ts::log::logger &    logger_;
    session_handler &    handler_;
};

} // namespace eztik

#endif

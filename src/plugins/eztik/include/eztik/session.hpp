#ifndef EZTIK_SESSION_HPP
#define EZTIK_SESSION_HPP

#include "routeros/api.hpp"
#include "routeros/sentence.hpp"

#include "log/logger.hpp"

#include <boost/asio/io_context.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>

namespace eztik {

class session;

class session_handler {
  public:
    virtual void on_close(const session &s) = 0;
};

class session_read_callback final {
    using response   = eztik::routeros::response_sentence;
    using error_code = boost::system::error_code;

  public:
    using callback =
        std::function<void(const error_code &, session &, response &&)>;

    session_read_callback(callback &&cb, bool permanent = false)
        : cb_ {std::move(cb)}, permanent_ {permanent} {
    }

    inline void operator()(const error_code &err, session &s, response &&resp) {
        cb_(err, s, std::move(resp));
    }

    auto is_permanent() const noexcept {
        return permanent_;
    }

  private:
    callback cb_;
    bool     permanent_;
};

class session final : public eztik::routeros::api_handler {
  public:
    session(std::uint32_t            id,
            boost::asio::io_context &io,
            ts::log::logger &        logger,
            session_handler &        handler)
        : id_ {id}, api_ {io, *this}, logger_ {logger}, handler_ {handler} {
    }

    inline auto id() const noexcept -> std::uint32_t {
        return id_;
    }

    inline auto api() noexcept -> eztik::routeros::api & {
        return api_;
    }

    void send(const eztik::routeros::request_sentence &sen,
              session_read_callback::callback &&       cb,
              bool                                     permanent = false);

    void add_read_callback(std::uint32_t                     tag,
                           session_read_callback::callback &&cb,
                           bool                              permanent);

    inline auto has_read_callback(std::uint32_t tag) const noexcept -> bool {
        return read_cbs_.contains(tag);
    }

  public:
    void on_error(const boost::system::error_code &err) override;
    void on_close() override;
    void on_response(const eztik::routeros::sentence &resp) override;

  private:
    std::uint32_t        id_;
    eztik::routeros::api api_;
    ts::log::logger &    logger_;
    session_handler &    handler_;

    std::unordered_map<std::uint32_t, session_read_callback> read_cbs_;
};

} // namespace eztik

#endif

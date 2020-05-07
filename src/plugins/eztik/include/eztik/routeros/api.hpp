#ifndef EZTIK_ROUTEROS_API_HPP
#define EZTIK_ROUTEROS_API_HPP

#include "eztik/routeros/api_read_state_machine.hpp"
#include "eztik/routeros/sentence.hpp"

#include "log/logger.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <string>
#include <unordered_map>

namespace eztik::routeros {

class api;

class api_handler {
  public:
    virtual void on_error(const boost::system::error_code &err) = 0;
};

class api_read_callback final {
    using response   = eztik::routeros::response_sentence;
    using error_code = boost::system::error_code;

  public:
    using callback =
        std::function<void(const error_code &, api &, response &&)>;

    api_read_callback(callback &&cb, bool permanent = false)
        : cb_ {std::move(cb)}, permanent_ {permanent} {
    }

    inline void operator()(const error_code &err, api &ros, response &&resp) {
        cb_(err, ros, std::move(resp));
    }

    auto is_permanent() const noexcept {
        return permanent_;
    }

  private:
    callback cb_;
    bool     permanent_;
};

class api final : public api_read_state_machine {
    using connect_handler =
        std::function<void(const boost::system::error_code &)>;
    using send_handler  = std::function<void(const boost::system::error_code &,
                                            const std::size_t &)>;
    using login_handler = std::function<void(bool)>;

  public:
    api(const std::uint32_t      id,
        boost::asio::io_context &io,
        ts::log::logger &        logger,
        api_handler &            handler)
        : id_ {id},
          sock_ {io},
          logger_ {logger},
          handler_ {handler},
          current_tag_ {0} {
    }

    api(const api &rh) = delete;

    void
    open(const std::string &host, std::uint16_t port, connect_handler &&cb);

    void close();

    void send(const eztik::routeros::request_sentence &sen,
              api_read_callback::callback &&           cb,
              bool                                     permanent = false);

    void login(const std::string &username,
               const std::string &password,
               login_handler &&   cb);

    inline auto make_request(std::string command)->request_sentence {
        request_sentence ret{std::move(command), current_tag_++};
        return ret;
    }

    inline auto is_ready() const noexcept -> bool {
        return sock_.is_open() && state() != read_state::closed;
    }

    void on_reading_length(std::string_view data) override;
    void on_reading_word(std::string_view data) override;
    void on_error(const boost::system::error_code &err) override;

  private:
    void start();

    void read_next(std::size_t n);
    void read_next_word();
    void handle_response(const sentence &s);

  private:
    const std::uint32_t          id_;
    boost::asio::ip::tcp::socket sock_;
    ts::log::logger &            logger_;
    api_handler &                handler_;
    std::uint32_t                current_tag_;

    std::unordered_map<std::uint32_t, api_read_callback> read_cbs_;
};

} // namespace eztik::routeros

#endif

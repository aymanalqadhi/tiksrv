#ifndef EZTIK_ROUTEROS_API_HPP
#define EZTIK_ROUTEROS_API_HPP

#include "eztik/routeros/sentence.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <atomic>
#include <string>
#include <vector>

namespace eztik::routeros {

class api_handler {
  public:
    virtual void on_error(const boost::system::error_code &err) = 0;
    virtual void on_close();
    virtual void on_response(const sentence &resp);
};

class api final {
    using connect_handler =
        std::function<void(const boost::system::error_code &)>;
    using send_handler = std::function<void(const boost::system::error_code &,
                                            const std::size_t &)>;

  public:
    api(boost::asio::io_context &io, api_handler &handler)
        : sock_ {io}, handler_ {handler}, running_ {false} {
    }

    api(api &&rh) : sock_ {std::move(rh.sock_)}, handler_ {rh.handler_} {
        running_.store(rh.running_);
    }

    api(const api &rh) = delete;

    ~api() {
        if (is_running()) {
            stop();
        }

        if (is_open()) {
            close();
        }
    }

    void open(const std::string &    host,
              std::uint16_t          port,
              const connect_handler &cb);

    void close();

    void send(const sentence &s, const send_handler &cb);

    void start();

    void stop();

    inline auto is_running() const noexcept -> bool {
        return running_.load();
    }

    inline auto is_open() const noexcept -> bool {
        return sock_.is_open();
    }

  private:
    std::vector<std::uint8_t>    buffer_;
    boost::asio::ip::tcp::socket sock_;
    std::atomic_bool             running_;
    api_handler &                handler_;
};

} // namespace eztik::routeros

#endif

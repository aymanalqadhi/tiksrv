#ifndef EZTIK_ROUTEROS_API_HPP
#define EZTIK_ROUTEROS_API_HPP

#include "eztik/routeros/api_read_state_machine.hpp"
#include "eztik/routeros/sentence.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <vector>

namespace eztik::routeros {

class api_handler {
  public:
    virtual void on_error(const boost::system::error_code &err) = 0;
    virtual void on_close()                                     = 0;
    virtual void on_response(const sentence &resp)              = 0;
};

class api final : public api_read_state_machine {
    using connect_handler =
        std::function<void(const boost::system::error_code &)>;
    using send_handler = std::function<void(const boost::system::error_code &,
                                            const std::size_t &)>;

  public:
    api(boost::asio::io_context &io, api_handler &handler)
        : sock_ {io}, handler_ {handler} {
    }

    api(api &&rh) : sock_ {std::move(rh.sock_)}, handler_ {rh.handler_} {
    }

    api(const api &rh) = delete;

    ~api() {
        if (is_open()) {
            close();
        }
    }

    void open(const std::string &    host,
              std::uint16_t          port,
              const connect_handler &cb);

    void close();

    void send(const request_sentence &s, const send_handler &cb);

    void start();

    void stop();

    inline auto is_open() const noexcept -> bool {
        return sock_.is_open();
    }

  public:
    void on_reading_length(std::string_view data) override;
    void on_reading_word(std::string_view data) override;
    void on_error(const boost::system::error_code &err) override;

  private:
    void read_next(std::size_t n);
    void read_next_word();

  private:
    std::vector<std::uint8_t>    buffer_;
    boost::asio::ip::tcp::socket sock_;
    api_handler &                handler_;
};

} // namespace eztik::routeros

#endif

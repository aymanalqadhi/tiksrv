#ifndef EZTIK_ROUTEROS_API_HPP
#define EZTIK_ROUTEROS_API_HPP

#include "eztik/routeros/api_read_state_machine.hpp"
#include "eztik/routeros/sentence.hpp"

#include "log/logger.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <deque>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace eztik::routeros {

class api_handler {
  public:
    virtual void on_error(const boost::system::error_code &err) = 0;
};

struct api_read_callback final {
    using response   = eztik::routeros::response_sentence;
    using error_code = boost::system::error_code;
    using callback   = std::function<bool(const error_code &, response &&)>;

    api_read_callback(callback &&cb) : cb_ {std::move(cb)} {
    }

    inline auto operator()(const error_code &err, response &&resp) -> bool {
        return cb_(err, std::move(resp));
    }

  private:
    callback cb_;
};

class api final : public api_read_state_machine,
                  public std::enable_shared_from_this<api> {
    using connect_handler =
        std::function<void(const boost::system::error_code &)>;
    using send_handler = std::function<void(const boost::system::error_code &)>;
    using login_handler =
        std::function<void(const boost::system::error_code &)>;

  public:
    ~api() {
        if (is_open()) {
            close();
        }
    }

    void
    open(const std::string &host, std::uint16_t port, connect_handler &&cb);

    void close();

    void send(std::shared_ptr<request_sentence> req,
              api_read_callback::callback &&    cb);

    void login(std::string username, std::string password, login_handler &&cb);

    inline auto is_open() const noexcept -> bool {
        return sock_.is_open() && state() != read_state::closed;
    }

    inline auto is_logged_in() const noexcept -> bool {
        return logged_in_;
    }

    inline auto aquire_unique_tag() noexcept -> std::uint32_t {
        return current_tag_++;
    }

    inline auto make_request(std::string command)
        -> std::shared_ptr<request_sentence> {
        return std::make_shared<request_sentence>(std::move(command),
                                                  aquire_unique_tag());
    }

    template <
        typename T,
        typename... TArg,
        typename = std::enable_if_t<std::is_base_of_v<request_sentence, T>>>
    inline auto make_command(TArg... args) -> std::shared_ptr<T> {
        return std::make_shared<T>(aquire_unique_tag(),
                                   std::forward<TArg>(args)...);
    }

    void on_reading_length(std::string_view data) override;

    void on_reading_word(std::string_view data) override;

    void on_error(const boost::system::error_code &err) override;

    static auto create(boost::asio::io_context &io, api_handler &h)
        -> std::shared_ptr<api> {
        return std::shared_ptr<api>(new api {io, h});
    }

  private:
    api(boost::asio::io_context &io, api_handler &handler)
        : io_ {io},
          sock_ {io},
          handler_ {handler},
          logged_in_ {false},
          current_tag_ {0} {
    }

    void start();

    void read_next(std::size_t n);

    void read_next_word();

    void handle_response(const sentence &s);

    void send_next();

    boost::asio::io_context &    io_;
    boost::asio::ip::tcp::socket sock_;

    api_handler & handler_;
    bool          logged_in_;
    std::uint32_t current_tag_;

    std::unordered_map<std::uint32_t, api_read_callback> read_cbs_;
    std::deque<std::pair<std::shared_ptr<request_sentence>, api_read_callback>>
        send_queue_;
};

} // namespace eztik::routeros

#endif

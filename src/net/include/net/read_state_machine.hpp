#ifndef TIKSRV_NET_READ_STATE_MACHINE_HPP
#define TIKSRV_NET_READ_STATE_MACHINE_HPP

#include "net/message.hpp"

#include <boost/system/error_code.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace ts::net {

enum class read_state { idle, reading_header, reading_body, closed };

class read_context final {
  public:
    [[nodiscard]] inline auto header() noexcept -> request_header & {
        return header_;
    }

    [[nodiscard]] inline auto buffer() noexcept -> std::string & {
        return buffer_;
    }

    [[nodiscard]] inline auto size() const noexcept -> std::size_t {
        return buffer_.size();
    }

    inline void resize(std::size_t n) {
        buffer_.resize(n);
    }

  private:
    request_header header_;
    std::string    buffer_;
};

class read_state_machine {
  public:
    [[nodiscard]] inline auto state() const noexcept -> const read_state & {
        return state_;
    }

    [[nodiscard]] inline auto context() noexcept -> read_context & {
        return ctx_;
    }

    inline void state(read_state new_state) {
        state_ = std::move(new_state);
    }

  protected:
    void handle_read(const boost::system::error_code &err,
                     std::string_view                 buf);

    virtual void on_reading_header(std::string_view data)       = 0;
    virtual void on_reading_body(std::string_view data)         = 0;
    virtual void on_error(const boost::system::error_code &err) = 0;

  private:
    read_state   state_ {read_state::idle};
    read_context ctx_;
};

} // namespace ts::net

#endif

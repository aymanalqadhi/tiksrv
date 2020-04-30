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
    read_context() : header_ {} {
    }

    inline request_header &header() {
        return header_;
    }

    inline std::string &buffer() noexcept {
        return buffer_;
    }

    inline std::size_t size() const noexcept {
        return buffer_.size();
    }

    inline void resize(std::size_t n) {
        buffer_ = {};
        buffer_.resize(n);
    }

  private:
    request_header header_;
    std::string    buffer_;
};

class read_state_machine {
  public:
    read_state_machine() : state_ {read_state::idle} {
    }

    inline const read_state &state() const noexcept {
        return state_;
    }

    inline read_context &context() noexcept {
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
    read_state   state_;
    read_context ctx_;
};

} // namespace ts::net

#endif

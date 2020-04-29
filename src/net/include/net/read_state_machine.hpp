#ifndef TIKSRV_NET_READ_STATE_MACHINE_HPP
#define TIKSRV_NET_READ_STATE_MACHINE_HPP

#include "net/message.hpp"

#include <boost/system/error_code.hpp>

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

namespace ts::net {

enum class read_state { idle, reading_header, reading_body, closed };

struct read_context {
    std::string    buffer;
    request_header header;

    inline void reinit() {
        buffer = {};
        header = {};
    }
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
    void handle_read(std::size_t                      nread,
                     std::string_view                 buf,
                     const boost::system::error_code &err);

    virtual void on_reading_header(std::string_view data)       = 0;
    virtual void on_reading_body(std::string_view data)         = 0;
    virtual void on_error(const boost::system::error_code &err) = 0;

  private:
    read_state   state_;
    read_context ctx_;
};

} // namespace ts::net

#endif

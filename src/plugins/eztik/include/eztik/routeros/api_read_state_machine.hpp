#ifndef EZTIK_ROUTEROS_READ_STATE_MACHINE_HPP
#define EZTIK_ROUTEROS_READ_STATE_MACHINE_HPP

#include "eztik/routeros/sentence.hpp"

#include <boost/system/error_code.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace eztik::routeros {

enum class read_state {
    idle,
    reading_length,
    reading_word,
    closed
};

class read_context final {
  public:
    read_context() : sentence_ {} {
    }

    inline auto length_buffer() noexcept -> std::array<std::uint8_t, 5> & {
        return length_buffer_;
    }

    inline auto buffer() noexcept -> std::string & {
        return buffer_;
    }

    inline auto size() const noexcept -> std::size_t {
        return buffer_.size();
    }

    inline auto current_sentence() noexcept -> sentence & {
        return sentence_;
    }

    inline void resize(std::size_t n) {
        buffer_ = {};
        buffer_.resize(n);
    }

  private:
    std::array<std::uint8_t, 5> length_buffer_;
    std::string                 buffer_;
    sentence                    sentence_;
};

class api_read_state_machine {
  public:
    api_read_state_machine() : state_ {read_state::closed} {
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

    virtual void on_reading_length(std::string_view data)       = 0;
    virtual void on_reading_word(std::string_view data)         = 0;
    virtual void on_error(const boost::system::error_code &err) = 0;

  private:
    void read_next_word();

    read_state   state_;
    read_context ctx_;
};

} // namespace eztik::routeros

#endif

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

enum class read_state { idle, reading_length, reading_word, closed };

class read_context final {
  public:
    read_context() : sentence_ {} {
    }

    [[nodiscard]] inline auto length_buffer() noexcept
        -> std::array<std::uint8_t, 5> & {
        return length_buffer_;
    }

    [[nodiscard]] inline auto buffer() noexcept -> std::string & {
        return buffer_;
    }

    [[nodiscard]] inline auto size() const noexcept -> std::size_t {
        return buffer_.size();
    }

    [[nodiscard]] inline auto current_sentence() noexcept -> sentence & {
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
    [[nodiscard]] inline auto state() const noexcept -> const read_state & {
        return state_;
    }

    [[nodiscard]] inline auto context() noexcept -> read_context &{
        return ctx_;
    }

    inline void state(read_state new_state) {
        state_ = new_state;
    }

  protected:
    void handle_read(const boost::system::error_code &err,
                     std::string_view                 buf);

    virtual void on_reading_length(std::string_view data)       = 0;
    virtual void on_reading_word(std::string_view data)         = 0;
    virtual void on_error(const boost::system::error_code &err) = 0;

  private:
    read_state   state_{read_state::closed};
    read_context ctx_;
};

} // namespace eztik::routeros

#endif

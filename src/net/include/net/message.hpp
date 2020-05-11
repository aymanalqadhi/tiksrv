#ifndef TIKSRV_NET_MESSAGE_HPP
#define TIKSRV_NET_MESSAGE_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

namespace ts::net {

struct request_header {
    static constexpr auto size = 16LU;

    std::uint32_t command;
    std::uint32_t flags;
    std::uint32_t tag;
    std::uint32_t body_size;

    void decode(const char *buf, std::size_t len);

    inline void decode(const std::array<std::uint8_t, size> &buf) {
        decode(reinterpret_cast<const char *>(buf.data()), buf.size());
    }
};

struct request final {
    request(request_header &&header, std::string &&body)
        : header_ {header}, body_ {std::move(body)} {
    }

    const request_header &header() const noexcept {
        return header_;
    }

    const std::string &body() noexcept {
        return body_;
    }

  private:
    request_header header_;
    std::string    body_;
};

struct response_header final {
    static constexpr auto size = 16LU;

    std::uint32_t code;
    std::uint32_t flags;
    std::uint32_t tag;
    std::uint32_t body_size;

  private:
    friend class response;

    void encode(std::array<std::uint8_t, size> &buf);
};

struct response final {
    auto header() noexcept -> response_header & {
        return header_;
    }

    auto header_buffer() noexcept
        -> std::array<std::uint8_t, response_header::size> & {
        return header_buf_;
    }

    void update_header_buffer() {
        header_.encode(header_buf_);
    }

    auto body() noexcept -> std::string & {
        return body_buf_;
    }

    void body(std::string value) noexcept {
        body_buf_         = std::move(value);
        header_.body_size = body_buf_.size();
    }

    void code(std::uint32_t value) noexcept {
        header_.code = value;
    }

    void flags(std::uint32_t value) noexcept {
        header_.flags = value;
    }

    void tag(std::uint32_t value) noexcept {
        header_.tag = value;
    }

  private:
    response_header header_;

    std::array<std::uint8_t, response_header::size> header_buf_;
    std::string                                     body_buf_;
};

enum class response_code : std::uint32_t {
    success           = 0,
    unknown_command   = 1,
    invalid_request   = 2,
    empty_request     = 3,
    operation_pending = 4,
    unknown_error     = 5
};

static_assert(sizeof(request_header) == request_header::size);
static_assert(sizeof(response_header) == response_header::size);

} // namespace ts::net

#endif

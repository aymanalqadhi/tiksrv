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

    void encode(std::array<std::uint8_t, size> &buf);
};

struct response final {
    response_header &header() noexcept {
        return header_;
    }

    std::string &body() noexcept {
        return body_;
    }

    void body(std::string value) noexcept {
        body_             = std::move(value);
        header_.body_size = body_.size();
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
    std::string     body_;
};

static_assert(sizeof(request_header) == request_header::size);
static_assert(sizeof(response_header) == response_header::size);

} // namespace ts::net

#endif

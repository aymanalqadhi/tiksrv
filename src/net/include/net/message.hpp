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

    void parse(const char *buf, std::size_t len);

    inline void parse(const std::array<std::uint8_t, size>& buf) {
        parse(reinterpret_cast<const char *>(buf.data()), buf.size());
    }

    inline void parse(const std::string& buf) {
        parse(buf.c_str(), buf.size());
    }
};

struct request final {
  public:
    request(request_header &&header, std::string &&body)
        : header_ {header}, body_ {std::move(body)} {
    }

    const request_header &header() const noexcept {
        return header_;
    }

    std::string &body() noexcept {
        return body_;
    }

  private:
    request_header header_;
    std::string    body_;
};

static_assert(sizeof(request_header) == request_header::size);

} // namespace ts::net

#endif

#ifndef TIKSRV_NET_MESSAGE_HPP
#define TIKSRV_NET_MESSAGE_HPP

#include <boost/endian/buffers.hpp>

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

namespace ts::net {

struct request_header {
    static constexpr auto size = 16LU;

    boost::endian::big_uint32_buf_t command;
    boost::endian::big_uint32_buf_t flags;
    boost::endian::big_uint32_buf_t tag;
    boost::endian::big_uint32_buf_t body_size;

    static request_header parse(const std::array<std::uint8_t, size> &buf);
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

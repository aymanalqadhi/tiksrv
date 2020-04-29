#include "net/message.hpp"

#include <boost/endian/buffers.hpp>

#include <array>
#include <cstdint>
#include <cstring>

namespace ts::net {

request_header
request_header::parse(const std::array<std::uint8_t, size> &buf) {
    request_header ret {};

    std::memcpy(reinterpret_cast<void *>(&ret), buf.data(), buf.size());
    ret.body_size.value();

    return ret;
}

} // namespace ts::net

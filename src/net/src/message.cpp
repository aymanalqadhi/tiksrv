#include "net/message.hpp"

#include <boost/endian.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace ts::net {

void request_header::decode(const char *buf, std::size_t len) {
    assert(len >= request_header::size);

    auto extract_integer = []<typename T>(const char **buf, T &out) {
        std::memcpy(reinterpret_cast<void *>(&out),
                    reinterpret_cast<const void *>(*buf), sizeof(T));
        boost::endian::big_to_native_inplace(out);
        *buf += sizeof(T);
    };

    extract_integer(&buf, command);
    extract_integer(&buf, flags);
    extract_integer(&buf, tag);
    extract_integer(&buf, body_size);
}

void response_header::encode(std::array<std::uint8_t, size> &buf) {
    auto ptr = reinterpret_cast<char *>(buf.data());

    auto append_integer = []<typename T>(char **buf, T value) {
        boost::endian::native_to_big_inplace(value);
        std::memcpy(*buf, &value, sizeof(T));
        *buf += sizeof(T);
    };

    append_integer(&ptr, code);
    append_integer(&ptr, flags);
    append_integer(&ptr, tag);
    append_integer(&ptr, body_size);
}

} // namespace ts::net

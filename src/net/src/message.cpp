#include "net/message.hpp"

#include <boost/endian.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace {

template <typename T>
inline void extract_number(const char **buf, T &out) {
    std::memcpy(reinterpret_cast<void *>(&out),
                reinterpret_cast<const void *>(*buf), sizeof(T));
    boost::endian::big_to_native_inplace(out);
    *buf += sizeof(T);
}

} // namespace

namespace ts::net {

void request_header::decode(const char *buf, std::size_t len) {
    assert(len >= request_header::size);

    ::extract_number(&buf, command);
    ::extract_number(&buf, flags);
    ::extract_number(&buf, tag);
    ::extract_number(&buf, body_size);
}

} // namespace ts::net

#include "eztik/routeros/sentence.hpp"

#include <boost/endian.hpp>

#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace {

template <typename T, std::size_t size = sizeof(T)>
inline void append_uint(const T &val, std::vector<std::uint8_t> &buf) {
    static_assert(size >= 1 && size <= 4);

    constexpr auto offset = 8 * (size - 1);

    if constexpr (std::endian::native == std::endian::big) {
        buf.push_back(val & 0xFF);
        if constexpr (size - 1 > 0) {
            append_uint<T, size - 1>(val >> 8, buf);
        }
    } else {
        buf.push_back((val >> offset) & 0xFF);
        if constexpr (size - 1 > 0) {
            append_uint<T, size - 1>(val, buf);
        }
    }
}

inline void encode_length(std::size_t len, std::vector<std::uint8_t> &buf) {
    if (len < 0x80) {
        append_uint<decltype(len), 1>(len, buf);
    } else if (len < 0x4000) {
        append_uint<decltype(len), 2>(len | 0x00008000, buf);
    } else if (len < 0x200000) {
        append_uint<decltype(len), 3>(len | 0x00C00000, buf);
    } else if (len < 0x10000000) {
        append_uint<decltype(len), 4>(len | 0xE0000000, buf);
    } else {
        append_uint<std::uint8_t>(0xF0, buf);
        append_uint<decltype(len), 4>(len, buf);
    }
}

inline void encode_word(const std::string &        word,
                        std::vector<std::uint8_t> &buf) {
    encode_length(word.size(), buf);
    std::copy(word.begin(), word.end(), std::back_inserter(buf));
}

inline void encode_param(const std::string &        key,
                         const std::string &        value,
                         std::vector<std::uint8_t> &buf) {
    encode_length(key.size() + value.size() + 2, buf);

    buf.push_back('=');
    std::copy(key.begin(), key.end(), std::back_inserter(buf));
    buf.push_back('=');

    std::copy(value.begin(), value.end(), std::back_inserter(buf));
}

} // namespace

namespace eztik::routeros {

std::vector<std::uint8_t> sentence::encode() {
    std::vector<std::uint8_t> outbuf {};

    encode_word(command_, outbuf);
    for (const auto &param : params_) {
        ::encode_param(param.first, param.second, outbuf);
    }
    outbuf.push_back(0x00);

    return outbuf;
}

} // namespace eztik::routeros

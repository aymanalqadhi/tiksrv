#include "eztik/routeros/sentence.hpp"

#include <boost/endian.hpp>
#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <cstdint>
#include <regex>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace {

template <typename T, std::size_t size = sizeof(T)>
inline void append_uint(const T &val, std::vector<std::uint8_t> &buf) {
    static_assert(size >= 1 && size <= 4 && size <= sizeof(T));

    buf.push_back((val >> (8 * (size - 1))) & 0xFF);
    if constexpr (size - 1 > 0) {
        append_uint<T, size - 1>(val, buf);
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

std::regex param_pattern {R"(=([^=]+)=(.*))", std::regex::extended};
std::regex tag_pattern {R"(\.tag=([0-9]+))", std::regex::extended};

inline auto is_valid_param(const std::string &str) -> bool {
    return std::regex_match(str, param_pattern);
}

inline auto is_valid_tag(const std::string &str) -> bool {
    return std::regex_match(str, tag_pattern);
}

inline void extract_param(const std::string &str, std::smatch &matches) {
    assert(is_valid_param(str));

    std::regex_match(str, matches, param_pattern);

    assert(matches.size() == 3);
}

inline auto extract_tag(const std::string &str) -> std::uint32_t {
    assert(is_valid_tag(str));

    std::uint32_t tag {};
    std::smatch   matches {};

    std::regex_match(str, matches, tag_pattern);
    assert(matches.size() == 2);

    return boost::lexical_cast<std::uint32_t>(matches[1].str());
}

} // namespace

namespace eztik::routeros {

void sentence::encode(std::vector<std::uint8_t> &vec) const {
    for (const auto &word : words_) {
        ::encode_word(word, vec);
    }
    vec.push_back(0x00);
}

auto response_sentence::is_valid_response(const sentence &s) -> bool {
    return s.size() > 0 && s[0][0] == '!';
}

response_sentence::response_sentence(const sentence &s) : tagged_{false} {
    assert(is_valid_response(s));

    auto type_str = s[0];

    if (type_str == "!fatal") {
        throw std::runtime_error {s.size() > 1 ? s[1] : "Unknown error"};
    }

    if (type_str == "!done") {
        type_ = response_sentence_type::normal;
    } else if (type_str == "!re") {
        type_ = response_sentence_type::data;
    } else if (type_str == "!trap") {
        type_ = response_sentence_type::trap;
    } else {
        throw std::runtime_error {"Invalid response type"};
    }

    std::smatch param_match {};

    for (const auto &word : s) {
        if (::is_valid_param(word)) {
            ::extract_param(word, param_match);
            params_.emplace(
                std::make_pair(param_match[1].str(), param_match[2].str()));
        } else if (::is_valid_tag(word)) {
            assert(!tagged_);
            tag_    = ::extract_tag(word);
            tagged_ = true;
        }
    }
}

} // namespace eztik::routeros

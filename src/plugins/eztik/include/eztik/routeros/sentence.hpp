#ifndef EZTIK_ROUTEROS_SENTENCE_HPP
#define EZTIK_ROUTEROS_SENTENCE_HPP

#include "fmt/format.h"

#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace eztik::routeros {

class sentence {
  public:
    inline auto size() const noexcept -> std::size_t {
        return words_.size();
    }

    inline auto operator[](std::size_t i) const noexcept
        -> const std::string & {
        return words_[i];
    }

    inline auto begin() const noexcept
        -> std::vector<std::string>::const_iterator {
        return words_.cbegin();
    }

    inline auto end() const noexcept
        -> std::vector<std::string>::const_iterator {
        return words_.end();
    }

    inline void push(std::string &&word) {
        words_.push_back(std::move(word));
    }

    inline void push(const std::string &word) {
        words_.push_back(word);
    }

    inline void clear() {
        words_.clear();
    }

    void encode(std::vector<std::uint8_t> &vec) const;

    template <typename... TArg>
    inline void push(const char *fmt, const TArg &... args) {
        words_.push_back(fmt::format(fmt, args...));
    }

  private:
    std::vector<std::string> words_;
};

class request_sentence final : public sentence {
  public:
    request_sentence(std::string command, std::uint32_t tag) : tag_ {tag} {
        push(command);
        push(".tag={}", tag);
    }

    inline auto tag() const noexcept -> std::uint32_t {
        return tag_;
    }

    inline void push_param(std::string key, std::string value) noexcept {
        sentence::push("={}={}", std::move(key), std::move(value));
    }

    template <typename T>
    inline void push_param(std::string key, const T &value) noexcept {
        push_param(std::move(key), std::to_string(value));
    }

    template <typename... TArg>
    inline void push_param(std::string key,
                           const char *fmt,
                           const TArg &... args) noexcept {
        push_param(std::move(key), fmt::format(fmt, args...));
    }

  private:
    std::uint32_t tag_;
};

enum response_sentence_type { normal, trap, data };

class response_sentence final {
  public:
    response_sentence() = default;

    response_sentence(const sentence &s);

    inline auto has(const std::string &key) const noexcept -> bool {
        return params_.contains(key);
    }

    inline auto operator[](const std::string &key) const noexcept
        -> const std::string & {
        return params_.at(key);
    }

    inline auto begin() const noexcept
        -> std::unordered_map<std::string, std::string>::const_iterator {
        return params_.begin();
    }

    inline auto end() const noexcept
        -> std::unordered_map<std::string, std::string>::const_iterator {
        return params_.end();
    }

    inline auto type() const noexcept -> const response_sentence_type & {
        return type_;
    }

    inline auto is_tagged() const noexcept -> bool {
        return tagged_;
    }

    inline auto tag() const noexcept -> std::uint32_t {
        assert(tagged_);
        return tag_;
    }

    static auto is_valid_response(const sentence &s) -> bool;

  private:
    std::unordered_map<std::string, std::string> params_;
    response_sentence_type                       type_;

    bool          tagged_;
    std::uint32_t tag_;
};

} // namespace eztik::routeros

#endif

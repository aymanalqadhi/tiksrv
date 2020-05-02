#ifndef EZTIK_ROUTEROS_SENTENCE_HPP
#define EZTIK_ROUTEROS_SENTENCE_HPP

#include "fmt/format.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace eztik::routeros {

class sentence {
  public:
    sentence(std::string command) : command_ {std::move(command)} {
    }

    sentence(sentence &&rt) {
        command_ = std::move(rt.command_);
        params_  = std::move(rt.params_);
    }

    auto command() const noexcept -> const std::string & {
        return command_;
    }

    inline void insert(std::string key, std::string value) noexcept {
        params_.insert(std::make_pair(std::move(key), std::move(value)));
    }

    template <typename T>
    inline void insert(std::string key, const T &value) noexcept {
        insert(std::move(key), std::to_string(value));
    }

    template <typename... TArg>
    inline void
    insert(std::string key, const char *fmt, const TArg &... args) noexcept {
        insert(std::move(key), fmt::format(fmt, args...));
    }

    inline auto begin() const noexcept {
        return params_.begin();
    }

    inline auto end() const noexcept {
        return params_.end();
    }

    inline auto find(const std::string &idx) const noexcept {
        return params_.find(idx);
    }

    inline auto erase(const std::string &idx) {
        return params_.erase(idx);
    }

    void encode(std::vector<std::uint8_t> &outbuf) const;

  private:
    std::string                                  command_;
    std::unordered_map<std::string, std::string> params_;
};

} // namespace eztik::routeros

#endif

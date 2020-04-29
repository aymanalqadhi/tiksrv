#ifndef TIKSRV_APP_SESSION_HPP
#define TIKSRV_APP_SESSION_HPP

#include "net/tcp_client.hpp"

#include <cstdint>
#include <memory>

namespace ts::app {

class session final {
  public:
    session(std::shared_ptr<ts::net::tcp_client> &&client)
        : client_ {std::move(client)} {
    }

    inline auto client() const noexcept {
        return client_;
    }

  private:
    std::shared_ptr<ts::net::tcp_client> client_;
};

} // namespace ts::app

#endif

#ifndef TIKSRV_INTEROP_COMMAND_HPP
#define TIKSRV_INTEROP_COMMAND_HPP

#include "net/tcp_client.hpp"

#include <memory>

namespace ts::interop {

class command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    virtual ~command() {
    }

    virtual auto name() const noexcept -> const std::string &          = 0;
    virtual void operator()(client_ptr client, ts::net::request &&req) = 0;
};

} // namespace ts::interop

#endif

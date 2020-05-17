#ifndef TIKSRV_INTEROP_COMMAND_HPP
#define TIKSRV_INTEROP_COMMAND_HPP

#include "net/tcp_client.hpp"

#include <memory>
#include <string>

namespace ts::interop {

class command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    command(std::string &&name) : name_ {std::move(name)} {
    }

    virtual ~command() = default;

    virtual void execute(client_ptr client, ts::net::request &&req) = 0;

    virtual void operator()(client_ptr client, ts::net::request &&req) {
        execute(std::move(client), std::move(req));
    }

    [[nodiscard]] auto name() const noexcept -> const std::string & {
        return name_;
    }

  private:
    std::string name_;
};

} // namespace ts::interop

#endif

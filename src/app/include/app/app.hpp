#ifndef TIKSRV_APP_APP_HPP
#define TIKSRV_APP_APP_HPP

#include "app/config.hpp"

#include "log/logger.hpp"

namespace ts::app {

class tiksrv_app final {
  public:
    tiksrv_app(const config &conf) : conf_ {conf}, logger_ {"app"} {
    }

    void run();

  private:
    void initialize();

  private:
    const config &  conf_;
    ts::log::logger logger_;
};

} // namespace ts::app

#endif

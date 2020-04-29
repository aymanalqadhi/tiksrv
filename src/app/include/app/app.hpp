#ifndef APP_APP_HPP
#define APP_APP_HPP

#include "app/config.hpp"

namespace ts::app {

class tiksrv_app final {
  public:
    tiksrv_app(const config &conf) : conf_ {conf} {
    }

    void run();

  private:
    const config &conf_;
};

} // namespace ts::app

#endif

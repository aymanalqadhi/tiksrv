#ifndef TIKSRV_TS_SERVICES_SERVICES_MANAGER_HPP
#define TIKSRV_TS_SERVICES_SERVICES_MANAGER_HPP

#include "services/service.hpp"

#include "config/config.hpp"
#include "log/logger.hpp"

#include <boost/asio/io_context.hpp>

#include <atomic>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

namespace ts::services {

enum class services_manager_state { uninitialized, initializing, initialized };

class services_manager final {
  public:
    services_manager(const ts::config::config &conf,
                     boost::asio::io_context & io)
        : conf_ {conf},
          io_ {io},
          state_ {services_manager_state::uninitialized},
          logger_ {"services_manager"},
          plugins_logger_ {"plugins"} {
    }

    inline auto config() const noexcept -> const ts::config::config & {
        return conf_;
    }

    inline auto io_context() noexcept -> boost::asio::io_context & {
        return io_;
    }

    inline auto logger() noexcept -> ts::log::logger & {
        return plugins_logger_;
    }

    template <typename T, typename = std::enable_if<is_service<T>::value>>
    inline auto has() -> bool {
        return services_.find(T::name) != services_.end();
    }

    template <typename T, typename = std::enable_if<is_service<T>::value>>
    inline auto get() -> std::shared_ptr<T> {
        return std::static_pointer_cast<T>(services_[T::name]);
    }

    template <typename T, typename = std::enable_if<is_service<T>::value>>
    void register_service(std::shared_ptr<T> service_ptr) {
        if (services_.contains(T::name)) {
            logger_.warn("Service `{}' is already registered, skipping",
                         T::name);
            return;
        }

        services_.insert(std::make_pair(T::name, std::move(service_ptr)));
    }

    void initialize();

  private:
    const ts::config::config &                      conf_;
    boost::asio::io_context &                       io_;
    std::map<std::string, std::shared_ptr<service>> services_;

    std::atomic<services_manager_state> state_;
    ts::log::logger                     logger_, plugins_logger_;
};

} // namespace ts::services

#endif

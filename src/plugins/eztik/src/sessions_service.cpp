#include "eztik/services/sessions.hpp"

#include "eztik/config.hpp"
#include "eztik/routeros/api.hpp"

#include "net/tcp_client.hpp"
#include "services/hooks_manager.hpp"

#include "spdlog/fmt/ostr.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <any>
#include <cstdint>
#include <memory>
#include <string>

using boost::asio::ip::tcp;
using boost::system::error_code;

namespace eztik::services {

void sessions_service::setup_hooks() {
    hooks_manager_->register_hook(
        ts::services::hooks_group::disconnection, [this](std::any data) {
            auto client =
                std::any_cast<std::shared_ptr<ts::net::tcp_client>>(data);

            if (sessions_.contains(client->id())) {
                logger_.debug("Deleting session #{}", client->id());
                sessions_.erase(client->id());
            }
        });
}

void sessions_service::create(std::uint32_t    id,
                              session_open_cb  open_cb,
                              session_close_cb close_cb) {

    assert(!has(id));

    std::string   ip;
    std::uint16_t port;

    if (!conf_.has(eztik::config_keys::ros_ip)) {
        logger_.warn("No RouterOS IP was configured, defaulting to {}",
                     eztik::config_defaults::ros_ip);
        ip = eztik::config_defaults::ros_ip;
    } else {
        ip = conf_[eztik::config_keys::ros_ip].as<decltype(ip)>();
    }

    if (!conf_.has(eztik::config_keys::ros_api_port)) {
        logger_.warn("No RouterOS API port was configured, defaulting to {}",
                     eztik::config_defaults::ros_api_port);
        port = eztik::config_defaults::ros_api_port;
    } else {
        port = conf_[eztik::config_keys::ros_api_port].as<decltype(port)>();
    }

    auto s = std::make_shared<session>(id, io_, logger_, *this);
    sessions_.emplace(
        std::make_pair(id, std::make_pair(s, std::move(close_cb))));

    s->api().open(ip, port, [id, s, this, open_cb](const error_code &err) {
        if (!sessions_.contains(id)) {
            return;
        }

        assert(!s->is_ready());

        if (err) {
            logger_.debug("Could not open session: {}", err.message());
            open_cb(nullptr);
            sessions_.erase(id);
        } else {
            open_cb(s);
            s->api().start();
            s->set_ready();
        }
    });
}

void sessions_service::close(std::uint32_t id) {
    assert(has(id));
    assert(operator[](id)->id() == id);

    logger_.info("Session #{} was closed", id);

    auto &to_remove = sessions_[id];

    to_remove.second(std::move(to_remove.first));
    sessions_.erase(id);
}

void sessions_service::on_close(const session &s) {
    assert(has(s.id()));
    close(s.id());
}

} // namespace eztik::services

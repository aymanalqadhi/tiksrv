#include "eztik/services/sessions.hpp"

#include "eztik/config.hpp"
#include "eztik/error.hpp"
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

using keys = eztik::config_keys;

namespace eztik::services {

void sessions_service::setup_hooks() {
    hooks_manager_->register_hook(
        ts::services::hooks_group::disconnection, [this](std::any data) {
            auto client =
                std::any_cast<std::shared_ptr<ts::net::tcp_client>>(data);

            if (sessions_.contains(client->id())) {
                logger_.debug("Deleting session #{}", client->id());
                close(client->id());
            }
        });
}

void sessions_service::create(std::uint32_t    id,
                              session_open_cb  open_cb,
                              session_close_cb close_cb) {
    assert(!has(id));

    auto ip   = conf_[keys::ros_ip].as<std::string>();
    auto port = conf_[keys::ros_api_port].as<std::uint16_t>();

    auto s = std::make_shared<session>(id, io_, logger_, *this);
    sessions_.emplace(
        std::make_pair(id, std::make_pair(s, std::move(close_cb))));

    s->api().open(
        ip, port,
        [this, id, s {std::move(s)}, cb {std::move(open_cb)}](const auto &err) {
            if (!has(id)) {
                return;
            }

            if (err) {
                assert(!s->is_ready());
                close(id);
                cb(err, nullptr);
            } else {
                assert(s->is_ready());

                auto user     = conf_[keys::ros_api_user].as<std::string>();
                auto password = conf_[keys::ros_api_password].as<std::string>();

                s->api().login(std::move(user), std::move(password),
                               [this, id, s {std::move(s)},
                                cb {std::move(cb)}](const auto &err) {
                                   if (!has(id)) {
                                       return;
                                   }

                                   if (err) {
                                       close(id);
                                       cb(err, nullptr);
                                   } else {
                                       cb(eztik::error_code::success,
                                          std::move(s));
                                   }
                               });
            }
        });
}

void sessions_service::close(std::uint32_t id) {
    assert(has(id));
    assert(operator[](id)->id() == id);

    logger_.info("Session #{} was closed", id);

    auto &to_remove = sessions_[id];

    if (to_remove.first->is_ready()) {
        to_remove.second(std::move(to_remove.first));
    }

    sessions_.erase(id);
}

void sessions_service::on_close(const session &s) {
    if (has(s.id())) {
        close(s.id());
    }
}

} // namespace eztik::services

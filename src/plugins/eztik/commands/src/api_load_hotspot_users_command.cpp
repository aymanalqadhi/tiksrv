#include "eztik/commands/api_load_hotspot_users.hpp"
#include "eztik/commands/response.hpp"

#include "eztik/data/api_repository.hpp"
#include "eztik/data/models/hotspot_user.hpp"

#include "net/message.hpp"

namespace eztik::commands {

void api_load_hotspot_users_command::execute(client_ptr         client,
                                             ts::net::request &&req) {
    eztik::data::api_repository<eztik::data::models::hotspot_user> repo {
        session_->api()};

    repo.load_stream([this, client {std::move(client)}](
                         const auto &err, auto &&user, auto is_last) {
        if (err) {
            logger_.error("An error occured while loading hotspot users: {}",
                          err.message());
            return;
        }

        if (is_last) {
            logger_.fatal("Last one, whew");
            return;
        }

        logger_.error("[+] Name: {}, Password: {}", user.name, user.password);
    });
}

} // namespace eztik::commands

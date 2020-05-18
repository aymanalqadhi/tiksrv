#ifndef EZTIK_COMMANDS_API_LOAD_HPP
#define EZTIK_COMMANDS_API_LOAD_HPP

#include "eztik/commands/session_required.hpp"

#include "eztik/data/api_repository.hpp"
#include "eztik/services/sessions.hpp"

#include "log/logger.hpp"
#include "net/message.hpp"

#include <string>

namespace eztik::commands::api {

template <typename D, typename TModel>
class load_command : public session_required_command {
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    load_command(std::string &&                     name,
                 ts::log::logger &                  logger,
                 eztik::services::sessions_service &sessions_svc)
        : session_required_command {std::move(name), logger, sessions_svc} {
    }

    inline auto create_response(TModel &&model) -> decltype(auto) {
        return D::create_response(std::move(model));
    }

    void execute(client_ptr client, ts::net::request &&req) override {
        eztik::data::api_repository<TModel> repo {current_session()->api()};

        repo.load_stream([this, client {std::move(client)},
                          req {std::move(req)}](const auto &err, auto &&model,
                                                auto is_last) {
            if (err) {
                logger().error("Could not load `{}': {}", TModel::api_path,
                               err.message());
                return;
            }

            if (is_last) {
                client->respond(ts::net::response_code::success,
                                req.header().tag);
                return;
            }

            client->respond(
                create_response(std::move(model)).SerializeAsString(),
                req.header().tag);
        });
    }
};

} // namespace eztik::commands::api

#endif

#ifndef EZTIK_DATA_API_REPOSITORY_HPP
#define EZTIK_DATA_API_REPOSITORY_HPP

#include "eztik/data/type_traits.hpp"

#include "eztik/routeros/api.hpp"
#include "eztik/routeros/commands.hpp"

#include "fmt/format.h"

#include <boost/system/error_code.hpp>

#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

namespace eztik::data {

template <typename TModel,
          typename = std::enable_if_t<is_api_model<TModel>::value>>
class api_repository {
    using load_handler = std::function<void(
        const boost::system::error_code &err, TModel &&, bool)>;
    using load_all_handler =
        std::function<void(const boost::system::error_code &,
                           std::shared_ptr<std::vector<TModel>>)>;

  public:
    api_repository(std::shared_ptr<eztik::routeros::api> api)
        : api_ {std::move(api)} {
    }

    void load_stream(load_handler &&cb) {
        assert(api_->is_open());

        auto req = api_->make_command<eztik::routeros::commands::getall>(
            TModel::api_path);

        api_->send(std::move(req),
                   [cb {std::move(cb)}](const auto &err, auto &&resp) {
                       if (err) {
                           cb(err, {}, true);
                           return false;
                       }

                       if (resp.empty()) {
                           cb({}, {}, true);
                           return false;
                       }

                       cb({}, TModel::create(resp), false);
                       return true;
                   });
    }

    void load_all(load_all_handler &&cb) {
        assert(api_->is_open());

        auto req = api_->make_command<eztik::routeros::commands::getall>(
            TModel::api_path);

        api_->send(std::move(req),
                   [ret = std::make_shared<std::vector<TModel>>(),
                    cb {std::move(cb)}](const auto &err, auto &&resp) {
                       if (err) {
                           cb(err, nullptr);
                           return false;
                       }

                       if (resp.empty()) {
                           cb({}, std::move(ret));
                           return false;
                       }

                       ret->emplace_back(TModel::create(resp));
                       return true;
                   });
    }

  private:
    std::shared_ptr<eztik::routeros::api> api_;
};

} // namespace eztik::data

#endif

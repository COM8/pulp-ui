#include "PulpHelper.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

namespace backend::pulp {
Config::Config(std::string&& remote, std::string&& username, std::string&& password) : remote(std::move(remote)), username(std::move(username)), password(std::move(password)) {}

namespace core {
void Users(const Config& /*config*/) {}
}  // namespace core

namespace rpm {
void Packages(const Config& /*config*/) {}
}  // namespace rpm
}  // namespace backend::pulp

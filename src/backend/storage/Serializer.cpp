#include "backend/storage/Serializer.hpp"
#include "backend/storage/Settings.hpp"
#include "nlohmann/json_fwd.hpp"

#include <chrono>
namespace backend::storage {
void to_json(nlohmann::json& j, const SettingsData& d) {
    j = nlohmann::json{
        {"pulp", {{"remote", d.pulp.remote}, {"username", d.pulp.username}, {"password", d.pulp.password}}}};
}

void from_json(const nlohmann::json& j, SettingsData& d) {
    nlohmann::json jPulp = j.at("pulp");
    jPulp.at("remote").get_to(d.pulp.remote);
    jPulp.at("username").get_to(d.pulp.username);
    jPulp.at("password").get_to(d.pulp.password);
}
}  // namespace backend::storage

namespace nlohmann {
void adl_serializer<std::chrono::system_clock::time_point>::to_json(json& j, const std::chrono::system_clock::time_point& tp) {
    j = std::chrono::system_clock::to_time_t(tp);
}

void adl_serializer<std::chrono::system_clock::time_point>::from_json(const json& j, std::chrono::system_clock::time_point& tp) {
    std::time_t t = 0;
    j.get_to(t);
    tp = std::chrono::system_clock::from_time_t(t);
}
}  // namespace nlohmann

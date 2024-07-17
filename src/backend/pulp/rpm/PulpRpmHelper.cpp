#include "PulpRpmHelper.hpp"
#include "backend/storage/Settings.hpp"
#include "cpr/auth.h"
#include "spdlog/spdlog.h"
#include <chrono>
#include <cstdint>
#include <expected>
#include <nlohmann/json.hpp>
#include <vector>
#include <cpr/cpr.h>
#include <fmt/color.h>

namespace backend::pulp::rpm {
template <typename T>
void parse_to(const nlohmann::json& j, const std::string& name, T& to) {
    if (!j.contains(name)) {
        throw std::runtime_error("Key '" + name + "' not found inside JSON message.");
    }
    j.at(name).get_to(to);
}

RpmPackage RpmPackage::from_json(const nlohmann::json& j) {
    RpmPackage package{};

    parse_to(j, "arch", package.arch);
    parse_to(j, "rpm_license", package.license);
    parse_to(j, "rpm_group", package.group);
    parse_to(j, "name", package.name);
    parse_to(j, "release", package.release);
    parse_to(j, "summary", package.summary);
    parse_to(j, "url", package.url);
    parse_to(j, "description", package.description);
    parse_to(j, "version", package.version);
    parse_to(j, "rpm_vendor", package.vendor);

    parse_to(j, "size_package", package.sizeRpm);
    parse_to(j, "size_installed", package.sizeInstalled);

    uint64_t timeBuild{0};
    parse_to(j, "time_build", timeBuild);
    package.timeBuild = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(timeBuild));

    return package;
}

std::expected<std::vector<RpmPackage>, std::string> parse_response(const std::string& response) {
    std::string errorMsg;
    try {
        nlohmann::json j = nlohmann::json::parse(response);

        nlohmann::json::array_t resultsArray;
        parse_to(j, "results", resultsArray);

        std::vector<RpmPackage> result;
        for (const nlohmann::json& jPackage : resultsArray) {
            result.emplace_back(RpmPackage::from_json(jPackage));
        }

        SPDLOG_INFO("Found {0} RPM packages.", result.size());
        return result;

    } catch (nlohmann::json::parse_error& e) {
        errorMsg = fmt::format("JSON error parsing rpm packages from '{0}' with: {1}", response, e.what());
    } catch (std::runtime_error& e) {
        errorMsg = fmt::format("General error parsing rpm packages from '{0}' with: {1}", response, e.what());
    } catch (...) {
        errorMsg = fmt::format("Unknown error parsing rpm packages from '{0}'.", response);
    }
    SPDLOG_ERROR("{}", errorMsg);
    return std::unexpected(errorMsg);
}

std::expected<std::vector<RpmPackage>, std::string> get_packages(const backend::storage::Pulp& pulpSettings) {
    cpr::Response response = cpr::Get(cpr::Url{pulpSettings.remote + "/pulp/api/v3/content/rpm/packages/"}, cpr::Ssl(cpr::ssl::VerifyPeer{pulpSettings.verifyPeer}, cpr::ssl::VerifyHost{pulpSettings.verifyHost}), cpr::Authentication(pulpSettings.username, pulpSettings.password, cpr::AuthMode::BASIC));

    if (response.status_code != 200) {
        std::string errorMsg;
        if (response.status_code == 0) {
            errorMsg = fmt::format("Request failed with error code {} and message '{}'.", static_cast<int>(response.error.code), response.error.message);
        } else {
            errorMsg = fmt::format("Request failed with status code {}.", response.status_code);
        }
        SPDLOG_ERROR("{}", errorMsg);

        return std::unexpected(errorMsg);
    }
    return parse_response(response.text);
}
}  // namespace backend::pulp::rpm

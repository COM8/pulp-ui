#include "ConnectionStatus.hpp"
#include "backend/storage/Settings.hpp"
#include "cpr/ssl_options.h"
#include "nlohmann/json_fwd.hpp"
#include "spdlog/fmt/bundled/core.h"
#include <cassert>
#include <chrono>
#include <memory>
#include <optional>
#include <stdexcept>
#include <thread>
#include <vector>
#include <cpr/api.h>
#include <cpr/cpr.h>
#include <cpr/cprtypes.h>
#include <cpr/response.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

namespace backend::pulp::core {
ConnectionStatus::ConnectionStatus() : shouldRun(true),
                                       connectionThread(std::make_unique<std::thread>(&ConnectionStatus::thread_run, this)) {
}

ConnectionStatus::~ConnectionStatus() {
    shouldRun = false;
    assert(connectionThread);
    if (connectionThread->joinable()) {
        connectionThread->join();
    }
}

std::optional<StatusResponse> ConnectionStatus::check_connectivity() {
    const backend::storage::Settings* settings = backend::storage::get_settings_instance();

    cpr::Response response = cpr::Get(cpr::Url{settings->data.pulp.remote + "/pulp/api/v3/status/"}, cpr::Ssl(cpr::ssl::VerifyPeer{settings->data.pulp.verifyPeer}, cpr::ssl::VerifyHost{settings->data.pulp.verifyHost}));

    if (response.status_code != 200) {
        std::string errorMsg;
        if (response.status_code == 0) {
            errorMsg = fmt::format("Request failed with error code {} and message '{}'.", static_cast<int>(response.error.code), response.error.message);
        } else {
            errorMsg = fmt::format("Request failed with status code {}.", response.status_code);
        }
        SPDLOG_ERROR("{}", errorMsg);
        lastErrorMessage = errorMsg;

        return std::nullopt;
    }

    return parse_response(response.text);
}

void ConnectionStatus::thread_run() {
    SPDLOG_DEBUG("Connection status thread started.");

    std::chrono::system_clock::time_point lastCheck = std::chrono::system_clock::time_point::min();

    while (shouldRun) {
        if (std::chrono::system_clock::now() < (lastCheck + std::chrono::seconds(5))) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        lastStatusResponse = check_connectivity();
        if (lastStatusResponse) {
            set_status(Status::CONNECTED);
        } else {
            set_status(Status::FAILED);
        }
        lastCheck = std::chrono::system_clock::now();
    }
    SPDLOG_DEBUG("Connection status thread ended.");
}

[[nodiscard]] ConnectionStatus::Status ConnectionStatus::get_status() const {
    return status;
}

[[nodiscard]] std::chrono::system_clock::time_point ConnectionStatus::get_last_successful_connection() const {
    return lastSuccessfulConnection;
}

[[nodiscard]] std::optional<StatusResponse> ConnectionStatus::get_last_connection_response() const {
    return lastStatusResponse;
}

[[nodiscard]] std::optional<std::string> ConnectionStatus::get_last_error_message() const {
    return lastErrorMessage;
}

void ConnectionStatus::set_status(Status newStatus) {
    // Update the connection status time
    if (newStatus == Status::CONNECTED) {
        lastSuccessfulConnection = std::chrono::system_clock::now();
    }

    // Invoke the event handler:
    if (statusChanged) {
        statusChanged(newStatus);
    }
}

StatusVersion StatusVersion::from_json(const nlohmann::json& j) {
    StatusVersion result{};

    parse_to(j, "component", result.component);
    parse_to(j, "version", result.version);
    parse_to(j, "package", result.package);
    parse_to(j, "module", result.module);
    parse_to(j, "domain_compatible", result.domainCompatible);

    return result;
}

StatusDb StatusDb::from_json(const nlohmann::json& j) {
    StatusDb result{};

    parse_to(j, "connected", result.connected);

    return result;
}

StatusRedis StatusRedis::from_json(const nlohmann::json& j) {
    StatusRedis result{};

    parse_to(j, "connected", result.connected);

    return result;
}

StatusStorage StatusStorage::from_json(const nlohmann::json& j) {
    StatusStorage result{};

    parse_to(j, "total", result.total);
    parse_to(j, "free", result.free);
    parse_to(j, "used", result.used);

    return result;
}

StatusContentSettings StatusContentSettings::from_json(const nlohmann::json& j) {
    StatusContentSettings result{};

    parse_to(j, "content_origin", result.origin);
    parse_to(j, "content_path_prefix", result.pathPrefix);

    return result;
}

StatusResponse StatusResponse::from_json(const nlohmann::json& j) {
    StatusResponse result{};

    parse_to(j, "domain_enabled", result.domainEnabled);

    if (!j.contains("versions")) {
        throw std::runtime_error("Key 'versions' not found inside JSON message.");
    }
    nlohmann::json::array_t versionsJsonArray;
    j.at("versions").get_to(versionsJsonArray);
    for (const nlohmann::json& versionJson : versionsJsonArray) {
        result.versions.emplace_back(StatusVersion::from_json(versionJson));
    }

    if (!j.contains("database_connection")) {
        throw std::runtime_error("Key 'database_connection' not found inside JSON message.");
    }
    result.db = StatusDb::from_json(j.at("database_connection"));

    if (!j.contains("redis_connection")) {
        throw std::runtime_error("Key 'redis_connection' not found inside JSON message.");
    }
    result.redis = StatusRedis::from_json(j.at("redis_connection"));

    if (!j.contains("storage")) {
        throw std::runtime_error("Key 'storage' not found inside JSON message.");
    }
    result.storage = StatusStorage::from_json(j.at("storage"));

    if (!j.contains("content_settings")) {
        throw std::runtime_error("Key 'content_settings' not found inside JSON message.");
    }
    result.contentSettings = StatusContentSettings::from_json(j.at("content_settings"));

    return result;
}

std::optional<StatusResponse> ConnectionStatus::parse_response(const std::string& response) {
    try {
        nlohmann::json j = nlohmann::json::parse(response);
        return std::make_optional<StatusResponse>(StatusResponse::from_json(j));

    } catch (nlohmann::json::parse_error& e) {
        const std::string errorMsg = fmt::format("Error parsing connection status from '{0}' with: {1}", response, e.what());
        SPDLOG_ERROR("{}", errorMsg);
        lastErrorMessage = errorMsg;
        return std::nullopt;
    }
}

}  // namespace backend::pulp::core
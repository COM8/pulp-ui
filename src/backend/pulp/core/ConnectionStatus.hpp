#pragma once

#include <atomic>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <eventpp/callbacklist.h>

namespace backend::pulp::core {
struct StatusVersion {
    std::string component;
    std::string version;
    std::string package;
    std::string module;
    bool domainCompatible;

    static StatusVersion from_json(const nlohmann::json& j);
} __attribute__((aligned(128)));

struct StatusDb {
    bool connected;

    static StatusDb from_json(const nlohmann::json& j);
};

struct StatusRedis {
    bool connected;

    static StatusRedis from_json(const nlohmann::json& j);
};

struct StatusStorage {
    size_t total;
    size_t used;
    size_t free;

    static StatusStorage from_json(const nlohmann::json& j);
} __attribute__((aligned(32)));

struct StatusContentSettings {
    std::string origin;
    std::string pathPrefix;

    static StatusContentSettings from_json(const nlohmann::json& j);
} __attribute__((aligned(64)));

struct StatusResponse {
    std::vector<StatusVersion> versions;
    StatusDb db;
    StatusRedis redis;
    StatusStorage storage;
    StatusContentSettings contentSettings;
    bool domainEnabled;

    static StatusResponse from_json(const nlohmann::json& j);
} __attribute__((aligned(128)));

template <typename T>
void parse_to(const nlohmann::json& j, const std::string& name, T& to) {
    if (!j.contains(name)) {
        throw std::runtime_error("Key '" + name + "' not found inside JSON message.");
    }
    j.at(name).get_to(to);
}

class ConnectionStatus {
 public:
    enum class Status {
        // No connection has been tried yet
        DISCONNECTED,
        // Connecting failed
        FAILED,
        // We successfully connected
        CONNECTED
    };

 private:
    std::atomic<Status> status{Status::DISCONNECTED};
    std::atomic<std::chrono::system_clock::time_point> lastSuccessfulConnection{std::chrono::system_clock::time_point::min()};
    std::atomic_bool shouldRun;
    std::unique_ptr<std::thread> connectionThread;

    std::optional<StatusResponse> lastStatusResponse{std::nullopt};
    std::optional<std::string> lastErrorMessage{std::nullopt};

 public:
    // Event handler:
    eventpp::CallbackList<void(Status)> statusChanged;

    ConnectionStatus();
    ConnectionStatus(ConnectionStatus&&) = delete;
    ConnectionStatus(const ConnectionStatus&) = delete;
    ~ConnectionStatus();

    ConnectionStatus& operator=(ConnectionStatus&&) = delete;
    ConnectionStatus& operator=(const ConnectionStatus&) = delete;

    [[nodiscard]] Status get_status() const;
    [[nodiscard]] std::chrono::system_clock::time_point get_last_successful_connection() const;
    [[nodiscard]] std::optional<StatusResponse> get_last_connection_response() const;
    [[nodiscard]] std::optional<std::string> get_last_error_message() const;

 private:
    void thread_run();
    std::optional<StatusResponse> check_connectivity();
    void set_status(Status newStatus);
    std::optional<StatusResponse> parse_response(const std::string& response);
};
}  // namespace backend::pulp::core
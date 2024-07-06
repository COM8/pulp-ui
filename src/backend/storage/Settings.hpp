#pragma once

#include "backend/storage/Serializer.hpp"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace backend::storage {
struct Pulp {
    std::string remote = "https://192.168.3.9:8080";
    bool verifyHost{true};
    bool verifyPeer{true};

    std::string username = "admin";
    std::string password;
} __attribute__((aligned(128)));

struct SettingsData {
    Pulp pulp;
} __attribute__((aligned(128)));

class Settings {
 public:
    explicit Settings(const std::filesystem::path& configFilePath);

    SettingsData data{};
    void write_settings();

 private:
    storage::Serializer fileHandle;
};

Settings* get_settings_instance();
}  // namespace backend::storage

#include "backend/storage/Settings.hpp"
#include "backend/storage/Serializer.hpp"
#include <spdlog/spdlog.h>

namespace backend::storage {
Settings::Settings(const std::filesystem::path& configFilePath) : fileHandle(configFilePath) {
    SPDLOG_DEBUG("Initializing ConfigurationStorage, this should only happen once per execution.");
    if (std::filesystem::exists(configFilePath)) {
        SPDLOG_DEBUG("Configuration file '{}' exists, trying to read it...", configFilePath.string());
        if (fileHandle.read_in()) {
            data = fileHandle.js_int.get<SettingsData>();
            SPDLOG_DEBUG("Configuration loaded successfully.");
        }
    } else {
        SPDLOG_ERROR("Configuration file '{}' does not exist. Creating a new, empty one.", configFilePath.string());
        write_settings();
    }
}

void Settings::write_settings() {
    fileHandle.js_int = data;
    fileHandle.write_out();
}

Settings* get_settings_instance() {
    static Settings settingsInstance("pulp-ui-settings.json");
    return &settingsInstance;
}
}  // namespace backend::storage

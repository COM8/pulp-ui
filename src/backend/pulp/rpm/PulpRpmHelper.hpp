#pragma once

#include "backend/storage/Settings.hpp"
#include <chrono>
#include <expected>
#include <string>
#include <vector>

namespace backend::pulp::rpm {
struct RpmPackage {
    std::string name;
    std::string arch;
    std::string version;
    std::string release;
    std::string summary;
    std::string description;
    std::string license;
    std::string group;
    size_t sizeRpm;
    size_t sizeInstalled;
    std::chrono::system_clock::time_point timeBuild;

    static RpmPackage from_json(const nlohmann::json& j);
} __attribute__((aligned(128)));

std::expected<std::vector<RpmPackage>, std::string> get_packages(const backend::storage::Pulp& pulpSettings);
}  // namespace backend::pulp::rpm
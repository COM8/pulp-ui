#pragma once

#include "backend/storage/Settings.hpp"
#include <chrono>
#include <cstdint>
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
    std::string url;
    std::string vendor;
    std::string description;
    std::string license;
    std::string group;
    size_t sizeRpm;
    size_t sizeInstalled;
    std::chrono::system_clock::time_point timeBuild;

    static RpmPackage from_json(const nlohmann::json& j);
} __attribute__((aligned(128)));

struct RpmRepository {
    std::string name;
    std::string description;
    std::chrono::system_clock::time_point created;
    std::chrono::system_clock::time_point lastUpdated;
    std::chrono::system_clock::time_point timeBuild;
    bool autoPublish;
    std::string metadataSigningServiceRef;
    std::string packageSigningServiceRef;
    std::string packageSigningFingerprint;
    uint64_t retainRepoVersions;
    uint64_t retainPackageVersions;

    static RpmRepository from_json(const nlohmann::json& j);
} __attribute__((aligned(128)));

struct SigningService {
    std::string name;
    std::chrono::system_clock::time_point created;
    std::chrono::system_clock::time_point lastUpdated;
    std::string publicKey;
    std::string publicKeyFingerprint;
    std::string scriptPath;

    static SigningService from_json(const nlohmann::json& j);
} __attribute__((aligned(128)));

std::expected<std::vector<RpmPackage>, std::string> get_packages(const backend::storage::Pulp& pulpSettings);
std::expected<std::vector<RpmRepository>, std::string> get_repositories(const backend::storage::Pulp& pulpSettings);
std::expected<std::vector<SigningService>, std::string> get_signing_services(const backend::storage::Pulp& pulpSettings);
}  // namespace backend::pulp::rpm
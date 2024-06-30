#pragma once

#include <string>

namespace backend::pulp {
struct Config {
    std::string remote;
    std::string username;
    std::string password;

    Config(std::string&& remote, std::string&& username, std::string&& password);
    Config(const Config& other) = default;
    Config(Config&& old) = default;

    ~Config() = default;

    Config& operator=(const Config& other) = default;
    Config& operator=(Config&& other) = default;
} __attribute__((aligned(128)));

namespace core {
void Users(const Config& config);
}  // namespace core

namespace rpm {
void Packages(const Config& config);
}  // namespace rpm
}  // namespace backend::pulp
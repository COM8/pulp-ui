cmake_minimum_required(VERSION 3.27)

add_library(backend_pulp_core)
add_library(backend::pulp::core ALIAS backend_pulp_core)

target_sources(backend_pulp_core PRIVATE ConnectionStatus.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/ConnectionStatus.hpp)

target_link_libraries(backend_pulp_core PRIVATE logger cpr::cpr backend::storage fmt::fmt
    PUBLIC nlohmann_json::nlohmann_json eventpp::eventpp)

cmake_minimum_required(VERSION 3.27)

add_library(backend_pulp_rpm)
add_library(backend::pulp::rpm ALIAS backend_pulp_rpm)

target_sources(backend_pulp_rpm PRIVATE PulpRpmHelper.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/PulpRpmHelper.hpp)

target_link_libraries(backend_pulp_rpm PRIVATE logger cpr::cpr backend::storage fmt::fmt date::date
    PUBLIC nlohmann_json::nlohmann_json)

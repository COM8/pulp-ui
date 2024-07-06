cmake_minimum_required(VERSION 3.27)

add_subdirectory(core)

add_library(backend_pulp)
add_library(backend::pulp ALIAS backend_pulp)

target_sources(backend_pulp PRIVATE PulpHelper.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/PulpHelper.hpp)

target_link_libraries(backend_pulp PRIVATE logger cpr::cpr date::date date::date-tz
    PUBLIC nlohmann_json::nlohmann_json)

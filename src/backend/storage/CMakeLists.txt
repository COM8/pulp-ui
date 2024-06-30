cmake_minimum_required(VERSION 3.27)

add_library(backend_storage)
add_library(backend::storage ALIAS backend_storage)

target_sources(backend_storage PRIVATE Serializer.cpp
    Settings.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/Serializer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Settings.hpp)

target_compile_features(backend_storage PRIVATE cxx_std_17 cxx_constexpr)

target_link_libraries(backend_storage PUBLIC nlohmann_json::nlohmann_json logger)

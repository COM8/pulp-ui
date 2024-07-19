cmake_minimum_required(VERSION 3.27)

add_library(ui_utils)
add_library(ui::utils ALIAS ui_utils)

target_sources(ui_utils PRIVATE UiUtils.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/UiUtils.hpp)

target_link_libraries(ui_utils PUBLIC PkgConfig::GTKMM
    PRIVATE logger fmt::fmt)

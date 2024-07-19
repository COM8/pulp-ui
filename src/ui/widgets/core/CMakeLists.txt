cmake_minimum_required(VERSION 3.27)

add_library(ui_widgets_core)
add_library(ui::widgets::core ALIAS ui_widgets_core)

target_sources(ui_widgets_core PRIVATE ConnectionStatusWidget.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/ConnectionStatusWidget.hpp)

target_link_libraries(ui_widgets_core PUBLIC PkgConfig::GTKMM backend::pulp::core
    PRIVATE logger fmt::fmt ui::utils)
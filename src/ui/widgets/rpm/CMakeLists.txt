cmake_minimum_required(VERSION 3.27)

add_library(ui_widgets_rpm)
add_library(ui::widgets::rpm ALIAS ui_widgets_rpm)

target_sources(ui_widgets_rpm PRIVATE RpmListWidget.cpp
    RepoListWidget.cpp
    RpmPackageWidget.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/RpmListWidget.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RepoListWidget.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RpmPackageWidget.hpp)

target_link_libraries(ui_widgets_rpm PUBLIC PkgConfig::GTKMM backend::pulp::rpm
    PRIVATE logger fmt::fmt)
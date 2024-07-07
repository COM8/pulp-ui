cmake_minimum_required(VERSION 3.27)

add_subdirectory(core)
add_subdirectory(rpm)

add_library(ui_widgets)
add_library(ui::widgets ALIAS ui_widgets)

target_sources(ui_widgets PRIVATE SettingsWidget.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/src
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/SettingsWidget.hpp)

target_link_libraries(ui_widgets PUBLIC PkgConfig::GTKMM
    PRIVATE logger backend::storage)
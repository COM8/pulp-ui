cmake_minimum_required(VERSION 3.27)

add_library(ui_windows MainWindow.hpp
    MainWindow.cpp)

target_link_libraries(ui_windows PUBLIC PkgConfig::GTKMM ui::widgets ui::widgets::core ui::widgets::rpm
    PRIVATE logger)

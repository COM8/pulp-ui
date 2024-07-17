#pragma once

#include "RpmPackageWidget.hpp"
#include <vector>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/listbox.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::rpm {
class RpmListWidget : public Gtk::ScrolledWindow {
 private:
    Gtk::Box mainBox{Gtk::Orientation::VERTICAL};

    Gtk::Box toolbarBox{Gtk::Orientation::HORIZONTAL};
    Gtk::Button refreshBtn{};

    std::vector<RpmPackageWidget> packageRows{};
    Gtk::ListBox packagesList{};

 public:
    RpmListWidget();
    RpmListWidget(RpmListWidget&&) = default;
    RpmListWidget(const RpmListWidget&) = delete;
    RpmListWidget& operator=(RpmListWidget&&) = default;
    RpmListWidget& operator=(const RpmListWidget&) = delete;
    ~RpmListWidget() override = default;

 private:
    void prep_widget();

    void update_packages();

    //-----------------------------Events:-----------------------------
};
}  // namespace ui::widgets::rpm
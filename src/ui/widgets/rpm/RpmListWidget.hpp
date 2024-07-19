#pragma once

#include "RpmPackageWidget.hpp"
#include "backend/pulp/rpm/PulpRpmHelper.hpp"
#include <vector>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/listbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>

namespace ui::widgets::rpm {
class RpmListWidget : public Gtk::ScrolledWindow {
 private:
    Gtk::Box mainBox{Gtk::Orientation::VERTICAL};

    Gtk::Box toolbarBox{Gtk::Orientation::HORIZONTAL};
    Gtk::Button refreshBtn{};
    Gtk::Button uploadBtn{};
    Gtk::Button sortBtn{};

    Gtk::SearchEntry searchEntry{};

    std::vector<RpmPackageWidget> packageRows{};
    Gtk::ListBox packagesList{};

    bool sortAscending{true};

    std::vector<backend::pulp::rpm::RpmPackage> packages;

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
    void filter_packages();

    //-----------------------------Events:-----------------------------
    void on_refresh_clicked();
    void on_upload_clicked();
    void on_sort_clicked();
    void on_search_changed();
};
}  // namespace ui::widgets::rpm
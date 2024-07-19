#pragma once

#include "backend/pulp/rpm/PulpRpmHelper.hpp"
#include <gtkmm/box.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>

namespace ui::widgets::rpm {
class RpmPackageWidget {
 private:
    GtkWidget* expanderRow{nullptr};
    Gtk::Box moreInfoBox{Gtk::Orientation::VERTICAL};
    Gtk::Label moreInfoLabel{};
    Gtk::Label archLabel{};
    Gtk::Label licenseLabel{};
    Gtk::Label groupLabel{};
    Gtk::Box moreInfoRowBox{Gtk::Orientation::HORIZONTAL};

 public:
    RpmPackageWidget();
    RpmPackageWidget(RpmPackageWidget&&) = default;
    RpmPackageWidget(const RpmPackageWidget&) = delete;
    RpmPackageWidget& operator=(RpmPackageWidget&&) = default;
    RpmPackageWidget& operator=(const RpmPackageWidget&) = delete;
    ~RpmPackageWidget() = default;

    void set_package(const backend::pulp::rpm::RpmPackage& package);

    [[nodiscard]] GtkWidget* get_widget() const;

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
};
}  // namespace ui::widgets::rpm
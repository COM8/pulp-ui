#include "RpmPackageWidget.hpp"
#include "backend/pulp/rpm/PulpRpmHelper.hpp"
#include "spdlog/fmt/bundled/core.h"
#include "ui/utils/UiUtils.hpp"
#include <chrono>
#include <cmath>
#include <adwaita.h>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <gtkmm/enums.h>

namespace ui::widgets::rpm {
RpmPackageWidget::RpmPackageWidget() {
    prep_widget();
}

void RpmPackageWidget::prep_widget() {
    expanderRow = adw_expander_row_new();

    moreInfoBox.set_margin(10);
    adw_expander_row_add_row(ADW_EXPANDER_ROW(expanderRow), GTK_WIDGET(moreInfoBox.gobj()));

    moreInfoLabel.set_wrap(true);
    moreInfoLabel.set_wrap_mode(Pango::WrapMode::WORD);
    moreInfoLabel.set_selectable(true);
    moreInfoLabel.set_justify(Gtk::Justification::LEFT);
    moreInfoLabel.set_halign(Gtk::Align::START);
    moreInfoBox.append(moreInfoLabel);

    archLabel.set_tooltip_text("Architecture");
    archLabel.add_css_class("tag");
    archLabel.add_css_class("tag-arch");
    archLabel.set_valign(Gtk::Align::CENTER);
    moreInfoRowBox.append(archLabel);

    licenseLabel.set_margin_start(10);
    licenseLabel.set_tooltip_text("License");
    licenseLabel.add_css_class("tag");
    licenseLabel.add_css_class("tag-license");
    licenseLabel.set_valign(Gtk::Align::CENTER);
    moreInfoRowBox.append(licenseLabel);

    groupLabel.set_margin_start(10);
    groupLabel.set_tooltip_text("RPM Package Group");
    groupLabel.add_css_class("tag");
    groupLabel.add_css_class("tag-group");
    groupLabel.set_valign(Gtk::Align::CENTER);
    moreInfoRowBox.append(groupLabel);

    adw_expander_row_add_suffix(ADW_EXPANDER_ROW(expanderRow), GTK_WIDGET(moreInfoRowBox.gobj()));
}

GtkWidget* RpmPackageWidget::get_widget() const {
    return expanderRow;
}

void RpmPackageWidget::set_package(const backend::pulp::rpm::RpmPackage& package) {
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(expanderRow), package.name.c_str());
    adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(expanderRow), (package.version + "-" + package.release).c_str());

    const std::string sizeString = fmt::format("RPM: {0}, Installed: {1}", ::ui::to_unit_string(package.sizeRpm), ::ui::to_unit_string(package.sizeInstalled));
    moreInfoLabel.set_markup(fmt::format("<b>Description:</b>\n{0}\n\n<b>URL:</b>\n{1}\n\n<b>Vendor:</b>\n{2}\n\n<b>Build:</b>\n{3}\n\n<b>Size:</b>\n{4}", package.description, package.url, package.vendor, fmt::format("{:%c %Z}", std::chrono::floor<std::chrono::seconds>(package.timeBuild)), sizeString));

    archLabel.set_label(package.arch);
    licenseLabel.set_label(package.license);
    groupLabel.set_label(package.group);
}

//-----------------------------Events:-----------------------------
}  // namespace ui::widgets::rpm
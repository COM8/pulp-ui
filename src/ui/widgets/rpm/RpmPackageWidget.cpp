#include "RpmPackageWidget.hpp"
#include "backend/pulp/rpm/PulpRpmHelper.hpp"
#include <adwaita.h>
#include <fmt/core.h>
#include <gtkmm/enums.h>

namespace ui::widgets::rpm {
RpmPackageWidget::RpmPackageWidget() {
    prep_widget();
}

void RpmPackageWidget::prep_widget() {
    expanderRow = adw_expander_row_new();
    moreInfoLabel.set_wrap(true);
    moreInfoLabel.set_wrap_mode(Pango::WrapMode::WORD);
    moreInfoLabel.set_selectable(true);
    moreInfoLabel.set_justify(Gtk::Justification::LEFT);
    moreInfoLabel.set_halign(Gtk::Align::START);
    moreInfoLabel.set_margin(10);
    adw_expander_row_add_row(ADW_EXPANDER_ROW(expanderRow), GTK_WIDGET(moreInfoLabel.gobj()));
}

GtkWidget* RpmPackageWidget::get_widget() const {
    return expanderRow;
}

void RpmPackageWidget::set_package(const backend::pulp::rpm::RpmPackage& package) {
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(expanderRow), package.name.c_str());
    adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(expanderRow), (package.version + "-" + package.release).c_str());

    moreInfoLabel.set_markup(fmt::format("<b>Description:</b>\n{0}\n<b>URL:</b>\n{1}", package.description, package.url));
}

//-----------------------------Events:-----------------------------
}  // namespace ui::widgets::rpm
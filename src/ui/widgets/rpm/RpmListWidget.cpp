#include "RpmListWidget.hpp"
#include "backend/pulp/rpm/PulpRpmHelper.hpp"
#include "backend/storage/Settings.hpp"
#include "ui/widgets/rpm/RpmPackageWidget.hpp"
#include <expected>
#include <vector>
#include <gtkmm/scrolledwindow.h>
#include <spdlog/spdlog.h>

namespace ui::widgets::rpm {
RpmListWidget::RpmListWidget() {
    prep_widget();
}

void RpmListWidget::prep_widget() {
    refreshBtn.set_icon_name("update-symbolic");
    refreshBtn.set_tooltip_text("Refresh RPM list");
    toolbarBox.append(refreshBtn);

    toolbarBox.add_css_class("toolbar");
    toolbarBox.add_css_class("card");
    toolbarBox.set_margin(10);
    mainBox.append(toolbarBox);

    packagesList.set_margin_start(10);
    packagesList.set_margin_end(10);
    packagesList.set_margin_bottom(10);
    packagesList.add_css_class("boxed-list");
    mainBox.append(packagesList);
    set_child(mainBox);

    update_packages();
}

void RpmListWidget::update_packages() {
    const backend::storage::Settings* settings = backend::storage::get_settings_instance();

    const std::expected<std::vector<backend::pulp::rpm::RpmPackage>, std::string> packages = backend::pulp::rpm::get_packages(settings->data.pulp);

    if (packages) {
        // Remove version list boxes in case there are now less
        while (packages->size() < packageRows.size()) {
            packagesList.remove(*Glib::wrap(packageRows.back().get_widget()));
            packageRows.pop_back();
        }

        // Add new ones until there are enough
        while (packages->size() > packageRows.size()) {
            packageRows.emplace_back();
            packagesList.append(*Glib::wrap(packageRows.back().get_widget()));
        }

        assert(packages->size() == packageRows.size());

        // Update all rows
        for (size_t i = 0; i < packages->size(); i++) {
            packageRows[i].set_package((*packages)[i]);
        }
    }
}

//-----------------------------Events:-----------------------------
}  // namespace ui::widgets::rpm
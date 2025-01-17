#include "RpmListWidget.hpp"
#include "backend/pulp/rpm/PulpRpmHelper.hpp"
#include "backend/storage/Settings.hpp"
#include "ui/utils/UiUtils.hpp"
#include "ui/widgets/rpm/RpmPackageWidget.hpp"
#include <expected>
#include <vector>
#include <gtkmm/enums.h>
#include <gtkmm/scrolledwindow.h>
#include <spdlog/spdlog.h>

namespace ui::widgets::rpm {
RpmListWidget::RpmListWidget() {
    prep_widget();
}

void RpmListWidget::prep_widget() {
    uploadBtn.set_icon_name("share-symbolic");
    uploadBtn.set_tooltip_text("Upload a new package");
    uploadBtn.signal_clicked().connect(sigc::mem_fun(*this, &RpmListWidget::on_upload_clicked));
    toolbarBox.append(uploadBtn);

    searchEntry.set_halign(Gtk::Align::FILL);
    searchEntry.set_hexpand(true);
    searchEntry.set_tooltip_text("Search for name or vendor");
    searchEntry.signal_changed().connect(sigc::mem_fun(*this, &RpmListWidget::on_search_changed));
    toolbarBox.append(searchEntry);

    sortBtn.set_icon_name("view-sort-ascending-symbolic");
    sortBtn.set_tooltip_text("Toggle sort direction");
    sortBtn.signal_clicked().connect(sigc::mem_fun(*this, &RpmListWidget::on_sort_clicked));
    toolbarBox.append(sortBtn);

    refreshBtn.set_icon_name("update-symbolic");
    refreshBtn.set_tooltip_text("Refresh RPM list");
    refreshBtn.set_halign(Gtk::Align::END);
    refreshBtn.signal_clicked().connect(sigc::mem_fun(*this, &RpmListWidget::on_refresh_clicked));
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

    const std::expected<std::vector<backend::pulp::rpm::RpmPackage>, std::string> packagesResult = backend::pulp::rpm::get_packages(settings->data.pulp);

    if (packagesResult) {
        packages = *packagesResult;
    }

    filter_packages();
}

void RpmListWidget::filter_packages() {
    const std::string filter = ui::to_lower_clean(searchEntry.get_text());
    std::string_view filterSv = filter;
    ui::trim(filterSv);

    std::vector<const backend::pulp::rpm::RpmPackage*> packagesFiltered{};

    // Filter
    for (const backend::pulp::rpm::RpmPackage& package : packages) {
        if (filterSv.empty() || ui::to_lower_clean(package.name).find(filterSv) != std::string::npos || ui::to_lower_clean(package.vendor).find(filterSv) != std::string::npos) {
            packagesFiltered.emplace_back(&package);
        }
    }

    // Sort
    if (sortAscending) {
        std::sort(packagesFiltered.begin(), packagesFiltered.end(), [](const backend::pulp::rpm::RpmPackage* a, const backend::pulp::rpm::RpmPackage* b) { return a->name < b->name; });
    } else {
        std::sort(packagesFiltered.begin(), packagesFiltered.end(), [](const backend::pulp::rpm::RpmPackage* a, const backend::pulp::rpm::RpmPackage* b) { return a->name > b->name; });
    }

    // Remove version list boxes in case there are now less
    while (packagesFiltered.size() < packageRows.size()) {
        packagesList.remove(*Glib::wrap(packageRows.back().get_widget()));
        packageRows.pop_back();
    }

    // Add new ones until there are enough
    while (packagesFiltered.size() > packageRows.size()) {
        packageRows.emplace_back();
        packagesList.append(*Glib::wrap(packageRows.back().get_widget()));
    }

    assert(packagesFiltered.size() == packageRows.size());

    // Update all rows
    for (size_t i = 0; i < packagesFiltered.size(); i++) {
        packageRows[i].set_package(*(packagesFiltered[i]));
    }
}

//-----------------------------Events:-----------------------------
void RpmListWidget::on_refresh_clicked() {
    update_packages();
}

void RpmListWidget::on_upload_clicked() {
}

void RpmListWidget::on_sort_clicked() {
    sortAscending = !sortAscending;
    sortBtn.set_icon_name(sortAscending ? "view-sort-ascending-symbolic" : "view-sort-descending-symbolic");
    filter_packages();
}

void RpmListWidget::on_search_changed() {
    filter_packages();
}
}  // namespace ui::widgets::rpm
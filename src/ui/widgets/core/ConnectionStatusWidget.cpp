#include "ConnectionStatusWidget.hpp"
#include "backend/pulp/core/ConnectionStatus.hpp"
#include "spdlog/fmt/bundled/core.h"
#include <chrono>
#include <cmath>
#include <adwaita.h>
#include <bits/chrono.h>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <gtkmm/enums.h>
#include <gtkmm/scrolledwindow.h>
#include <spdlog/spdlog.h>

namespace ui::widgets::core {
ConnectionStatusWidget::ConnectionStatusWidget() {
    prep_widget();

    connectionStatus.statusChanged.append([this](backend::pulp::core::ConnectionStatus::Status status) { this->on_connection_status_changed(status); });
}

void ConnectionStatusWidget::prep_widget() {
    mainBox.set_margin(10);
    set_child(mainBox);

    // Connection Status
    connectionStatusGroup = adw_preferences_group_new();
    AdwPreferencesGroup* connectionStatusGroupType = ADW_PREFERENCES_GROUP(connectionStatusGroup);
    adw_preferences_group_set_title(connectionStatusGroupType, "Connection Status");
    mainBox.append(*Glib::wrap(connectionStatusGroup));

    connectionStatusListBox.set_selection_mode(Gtk::SelectionMode::NONE);
    connectionStatusListBox.add_css_class("boxed-list");
    adw_preferences_group_add(connectionStatusGroupType, GTK_WIDGET(connectionStatusListBox.gobj()));

    statusCircle.set_draw_func(sigc::mem_fun(*this, &ConnectionStatusWidget::on_status_circle_draw));
    statusCircle.set_content_height(20);
    statusCircle.set_content_width(20);
    statusCircle.set_valign(Gtk::Align::CENTER);
    statusBox.set_margin_top(5);
    statusBox.append(statusCircle);

    statusLabel.set_margin_start(10);
    statusLabel.set_valign(Gtk::Align::CENTER);
    statusBox.append(statusLabel);
    statusBox.set_margin(10);

    connectionStatusListBox.append(statusBox);

    // Versions
    versionGroup = adw_preferences_group_new();
    gtk_widget_set_margin_top(versionGroup, 20);
    AdwPreferencesGroup* versionGroupType = ADW_PREFERENCES_GROUP(versionGroup);
    adw_preferences_group_set_title(versionGroupType, "Version");
    mainBox.append(*Glib::wrap(versionGroup));

    versionListBox.set_selection_mode(Gtk::SelectionMode::NONE);
    versionListBox.add_css_class("boxed-list");
    adw_preferences_group_add(versionGroupType, GTK_WIDGET(versionListBox.gobj()));

    // Connections
    connectionGroup = adw_preferences_group_new();
    gtk_widget_set_margin_top(connectionGroup, 20);
    AdwPreferencesGroup* connectionGroupType = ADW_PREFERENCES_GROUP(connectionGroup);
    adw_preferences_group_set_title(connectionGroupType, "Connection");
    mainBox.append(*Glib::wrap(connectionGroup));

    connectionListBox.set_selection_mode(Gtk::SelectionMode::NONE);
    connectionListBox.add_css_class("boxed-list");
    adw_preferences_group_add(connectionGroupType, GTK_WIDGET(connectionListBox.gobj()));

    dbConnectionRow = adw_action_row_new();
    AdwActionRow* dbConnectionRowType = ADW_ACTION_ROW(dbConnectionRow);
    gtk_widget_add_css_class(dbConnectionRow, "property");
    adw_action_row_set_subtitle_selectable(dbConnectionRowType, true);
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(dbConnectionRow), "DB");
    connectionListBox.append(*Glib::wrap(dbConnectionRow));

    redisConnectionRow = adw_action_row_new();
    AdwActionRow* redisConnectionRowType = ADW_ACTION_ROW(redisConnectionRow);
    gtk_widget_add_css_class(redisConnectionRow, "property");
    adw_action_row_set_subtitle_selectable(redisConnectionRowType, true);
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(redisConnectionRow), "Redis");
    connectionListBox.append(*Glib::wrap(redisConnectionRow));

    // Storage
    storageGroup = adw_preferences_group_new();
    gtk_widget_set_margin_top(storageGroup, 20);
    AdwPreferencesGroup* storageGroupType = ADW_PREFERENCES_GROUP(storageGroup);
    adw_preferences_group_set_title(storageGroupType, "Storage");
    mainBox.append(*Glib::wrap(storageGroup));

    storageListBox.set_selection_mode(Gtk::SelectionMode::NONE);
    storageListBox.add_css_class("boxed-list");
    adw_preferences_group_add(storageGroupType, GTK_WIDGET(storageListBox.gobj()));

    storageHeaderLabel.set_label("Usage");
    storageHeaderLabel.add_css_class("subtitle");
    storageHeaderLabel.set_halign(Gtk::Align::START);
    storageBox.append(storageHeaderLabel);

    storageProfessBar.remove_css_class("storage-pgb-ok");
    storageProfessBar.set_margin_top(5);
    storageBox.append(storageProfessBar);

    storageUsageLabel.set_margin_top(10);
    storageBox.append(storageUsageLabel);

    storageBox.set_margin(10);
    storageListBox.append(storageBox);

    // Content
    contentGroup = adw_preferences_group_new();
    gtk_widget_set_margin_top(contentGroup, 20);
    AdwPreferencesGroup* contentGroupType = ADW_PREFERENCES_GROUP(contentGroup);
    adw_preferences_group_set_title(contentGroupType, "Content");
    mainBox.append(*Glib::wrap(contentGroup));

    contentListBox.set_selection_mode(Gtk::SelectionMode::NONE);
    contentListBox.add_css_class("boxed-list");
    adw_preferences_group_add(contentGroupType, GTK_WIDGET(contentListBox.gobj()));

    contentOriginRow = adw_action_row_new();
    AdwActionRow* contentOriginRowType = ADW_ACTION_ROW(contentOriginRow);
    gtk_widget_add_css_class(contentOriginRow, "property");
    adw_action_row_set_subtitle_selectable(contentOriginRowType, true);
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(contentOriginRow), "Origin");
    contentListBox.append(*Glib::wrap(contentOriginRow));

    contentPathPrefixRow = adw_action_row_new();
    AdwActionRow* contentPathPrefixRowType = ADW_ACTION_ROW(contentPathPrefixRow);
    gtk_widget_add_css_class(contentPathPrefixRow, "property");
    adw_action_row_set_subtitle_selectable(contentPathPrefixRowType, true);
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(contentPathPrefixRow), "Path Prefix");
    contentListBox.append(*Glib::wrap(contentPathPrefixRow));

    update_status();
}

std::string ConnectionStatusWidget::to_unit_string(size_t byteCount) {
    std::string unit = "B";
    double unitVal = static_cast<double>(byteCount);
    if (unitVal >= 1024) {
        unitVal /= 1024;
        unit = "KB";
    }

    if (unitVal >= 1024) {
        unitVal /= 1024;
        unit = "MB";
    }

    if (unitVal >= 1024) {
        unitVal /= 1024;
        unit = "GB";
    }

    if (unitVal >= 1024) {
        unitVal /= 1024;
        unit = "TB";
    }
    return fmt::format("{} {}", std::round(unitVal * 100) / 100, unit);
}

void ConnectionStatusWidget::update_status() {
    // Connectivity status
    const std::chrono::system_clock::time_point lastSuccessfulConnection = connectionStatus.get_last_successful_connection();
    switch (status) {
        case backend::pulp::core::ConnectionStatus::Status::DISCONNECTED:
            statusLabel.set_text("Disconnected");
            if (lastSuccessfulConnection == std::chrono::system_clock::time_point::min()) {
                statusBox.set_tooltip_text("There wasn't yet a successful connection!");
            } else {
                statusBox.set_tooltip_text(fmt::format("Last successful connection: {:%T}", lastSuccessfulConnection));
            }
            break;

        case backend::pulp::core::ConnectionStatus::Status::CONNECTED:
            statusLabel.set_text("Connected");
            assert(lastSuccessfulConnection != std::chrono::system_clock::time_point::min());
            statusBox.set_tooltip_text(fmt::format("Last successful connection: {:%T}", lastSuccessfulConnection));
            break;

        case backend::pulp::core::ConnectionStatus::Status::FAILED:
            statusLabel.set_text("Failed");
            if (lastSuccessfulConnection == std::chrono::system_clock::time_point::min()) {
                statusBox.set_tooltip_text(fmt::format("There wasn't yet a successful connection!\nMessage: {}", *connectionStatus.get_last_error_message()));
            } else {
                statusBox.set_tooltip_text(fmt::format("Last successful connection: {:%T}\nMessage: {}", lastSuccessfulConnection, *connectionStatus.get_last_error_message()));
            }
            break;

        default:
            assert(false);  // should not happen
            break;
    }

    std::optional<backend::pulp::core::StatusResponse> response = connectionStatus.get_last_connection_response();

    // Version
    gtk_widget_set_visible(versionGroup, response != std::nullopt);
    if (response) {
        // Remove version list boxes in case there are now less
        while (response->versions.size() < versionRows.size()) {
            versionListBox.remove(*Glib::wrap(versionRows.back()));
            versionRows.pop_back();
        }

        // Add new ones until there are enough
        while (response->versions.size() > versionRows.size()) {
            GtkWidget* row = adw_action_row_new();
            AdwActionRow* rowType = ADW_ACTION_ROW(row);
            gtk_widget_add_css_class(row, "property");
            adw_action_row_set_subtitle_selectable(rowType, true);
            versionListBox.append(*Glib::wrap(row));
            versionRows.emplace_back(row);
        }

        assert(response->versions.size() == versionRows.size());

        // Update all rows
        for (size_t i = 0; i < response->versions.size(); i++) {
            adw_preferences_row_set_title(ADW_PREFERENCES_ROW(versionRows[i]), (response->versions[i].component + " - " + response->versions[i].package).c_str());
            adw_action_row_set_subtitle(ADW_ACTION_ROW(versionRows[i]), response->versions[i].version.c_str());
            gtk_widget_set_tooltip_text(versionRows[i], response->versions[i].module.c_str());
        }
    }

    // Connection
    gtk_widget_set_visible(connectionGroup, response != std::nullopt);
    if (response) {
        if (response->db.connected) {
            adw_action_row_set_subtitle(ADW_ACTION_ROW(dbConnectionRow), "Connected");
            gtk_widget_remove_css_class(dbConnectionRow, "error");
            gtk_widget_add_css_class(dbConnectionRow, "success");
        } else {
            adw_action_row_set_subtitle(ADW_ACTION_ROW(dbConnectionRow), "Disconnected");
            gtk_widget_remove_css_class(dbConnectionRow, "success");
            gtk_widget_add_css_class(dbConnectionRow, "error");
        }

        if (response->redis.connected) {
            adw_action_row_set_subtitle(ADW_ACTION_ROW(redisConnectionRow), "Connected");
            gtk_widget_remove_css_class(redisConnectionRow, "error");
            gtk_widget_add_css_class(redisConnectionRow, "success");
        } else {
            adw_action_row_set_subtitle(ADW_ACTION_ROW(redisConnectionRow), "Disconnected");
            gtk_widget_remove_css_class(redisConnectionRow, "success");
            gtk_widget_add_css_class(redisConnectionRow, "error");
        }
    }

    // Storage
    gtk_widget_set_visible(storageGroup, response != std::nullopt);
    if (response) {
        const double fraction = static_cast<double>(response->storage.used) / static_cast<double>(response->storage.total);

        if (fraction < 0.8) {
            storageProfessBar.add_css_class("storage-pgb-ok");
        } else if (fraction < 0.95) {
            storageProfessBar.add_css_class("storage-pgb-warn");
        } else {
            storageProfessBar.add_css_class("storage-pgb-alert");
        }

        storageProfessBar.set_fraction(fraction);
        storageProfessBar.set_text(fmt::format("Storage is {} full.", fraction * 100));

        storageUsageLabel.set_text(fmt::format("{} ({}%) used out of {} - {} free", to_unit_string(response->storage.used), std::round(fraction * 100), to_unit_string(response->storage.total), to_unit_string(response->storage.free)));
    }

    // Content
    gtk_widget_set_visible(contentGroup, response != std::nullopt);
    if (response) {
        adw_action_row_set_subtitle(ADW_ACTION_ROW(contentOriginRow), response->contentSettings.origin.c_str());
        adw_action_row_set_subtitle(ADW_ACTION_ROW(contentPathPrefixRow), response->contentSettings.pathPrefix.c_str());
    }
}

//-----------------------------Events:-----------------------------

void ConnectionStatusWidget::on_connection_status_changed(backend::pulp::core::ConnectionStatus::Status status) {
    this->status = status;
    statusCircle.queue_draw();
    update_status();
}

void ConnectionStatusWidget::on_status_circle_draw(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height) {
    ctx->save();

    Gdk::RGBA color;
    switch (status) {
        case backend::pulp::core::ConnectionStatus::Status::DISCONNECTED:
            color.set_rgba(0.8, 0.8, 0.8);
            break;

        case backend::pulp::core::ConnectionStatus::Status::CONNECTED:
            color.set_rgba(0.0, 1.0, 0.0);
            break;

        case backend::pulp::core::ConnectionStatus::Status::FAILED:
            color.set_rgba(1.0, 0.0, 0.0);
            break;

        default:
            assert(false);  // should not happen
            break;
    }

    // Create a radial gradient
    auto gradient = Cairo::RadialGradient::create(width, height, 2, width, width, width);
    gradient->add_color_stop_rgba(0, color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
    gradient->add_color_stop_rgba(1, color.get_red() * 0.8, color.get_green() * 0.8, color.get_blue() * 0.8, color.get_alpha() * 0.8);

    ctx->set_source(gradient);
    ctx->arc(width / 2.0, height / 2.0, std::min(width, height) / 2.0, 0, 2 * M_PI);  // Draw circle with radius 10
    ctx->fill();

    ctx->restore();
}
}  // namespace ui::widgets::core
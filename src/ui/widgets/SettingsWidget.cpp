#include "SettingsWidget.hpp"
#include "backend/storage/Serializer.hpp"
#include "backend/storage/Settings.hpp"
#include "logger/Logger.hpp"
#include <cstdlib>
#include <string>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/object.h>
#include <gtkmm/scrolledwindow.h>
#include <spdlog/spdlog.h>

namespace ui::widgets {
SettingsWidget::SettingsWidget() : Gtk::Box(Gtk::Orientation::VERTICAL) {
    prep_widget();
    load_settings();
    set_margin_start(10);
    set_margin_end(10);
}

void SettingsWidget::prep_widget() {
    // Save:
    Gtk::Button* saveBtn = Gtk::make_managed<Gtk::Button>("Save");
    saveBtn->signal_clicked().connect(sigc::mem_fun(*this, &SettingsWidget::on_save_clicked));
    Glib::RefPtr<Gtk::StyleContext> styleCtx = saveBtn->get_style_context();
    styleCtx->add_class("suggested-action");
    saveBtn->set_halign(Gtk::Align::END);
    saveBtn->set_margin_top(10);
    saveBtn->set_tooltip_text("Save settings");
    append(*saveBtn);

    // Content:
    Gtk::ScrolledWindow* scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_margin_top(10);
    scroll->set_vexpand(true);
    append(*scroll);
    Gtk::Box* contentBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    scroll->set_child(*contentBox);
    contentBox->set_margin_bottom(10);

    // Pulp:
    Gtk::Label* pulpRemoteLabel = Gtk::make_managed<Gtk::Label>();
    pulpRemoteLabel->set_text("Remote:");
    pulpRemoteLabel->set_margin_top(10);
    pulpRemoteLabel->set_halign(Gtk::Align::START);
    contentBox->append(*pulpRemoteLabel);
    pulpRemote.set_placeholder_text("https://example.com:8080");
    contentBox->append(pulpRemote);

    Gtk::Label* pulpUsernameLabel = Gtk::make_managed<Gtk::Label>();
    pulpUsernameLabel->set_text("Username:");
    pulpUsernameLabel->set_margin_top(10);
    pulpUsernameLabel->set_halign(Gtk::Align::START);
    contentBox->append(*pulpUsernameLabel);
    pulpUsername.set_placeholder_text("admin");
    contentBox->append(pulpUsername);

    Gtk::Label* pulpPasswordLabel = Gtk::make_managed<Gtk::Label>();
    pulpPasswordLabel->set_text("Password:");
    pulpPasswordLabel->set_margin_top(10);
    pulpPasswordLabel->set_halign(Gtk::Align::START);
    contentBox->append(*pulpPasswordLabel);
    contentBox->append(pulpPassword);
}

void SettingsWidget::load_settings() {
    SPDLOG_INFO("Loading settings...");
    const backend::storage::SettingsData* settings = &(backend::storage::get_settings_instance()->data);
    pulpRemote.set_text(settings->pulp.remote);
    pulpUsername.set_text(settings->pulp.username);
    pulpPassword.set_text(settings->pulp.password);
    SPDLOG_INFO("Settings loaded.");
}

void SettingsWidget::save_settings() {
    SPDLOG_INFO("Saving settings...");
    backend::storage::SettingsData* settings = &(backend::storage::get_settings_instance()->data);

    settings->pulp.remote = pulpRemote.get_text();
    settings->pulp.username = pulpUsername.get_text();
    settings->pulp.password = pulpPassword.get_text();

    backend::storage::get_settings_instance()->write_settings();
    SPDLOG_INFO("Settings saved.");
}

//-----------------------------Events:-----------------------------
void SettingsWidget::on_save_clicked() {
    save_settings();
}
}  // namespace ui::widgets
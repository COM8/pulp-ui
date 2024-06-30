#pragma once

#include <gtkmm.h>
#include <gtkmm/entry.h>
#include <gtkmm/passwordentry.h>

namespace ui::widgets {
class SettingsWidget : public Gtk::Box {
 private:
    Gtk::Entry pulpRemote;
    Gtk::Entry pulpUsername;
    Gtk::PasswordEntry pulpPassword;

 public:
    SettingsWidget();
    SettingsWidget(SettingsWidget&&) = default;
    SettingsWidget(const SettingsWidget&) = delete;
    SettingsWidget& operator=(SettingsWidget&&) = default;
    SettingsWidget& operator=(const SettingsWidget&) = delete;
    ~SettingsWidget() override = default;

 private:
    void prep_widget();
    void load_settings();
    void save_settings();

    //-----------------------------Events:-----------------------------
    void on_save_clicked();
};
}  // namespace ui::widgets
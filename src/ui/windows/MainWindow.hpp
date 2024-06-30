#pragma once

#include <gdkmm/enums.h>
#include <glib/gtypes.h>
#include <gtkmm.h>
#include <gtkmm/window.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
 public:
    MainWindow();
    MainWindow(MainWindow&&) = delete;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    ~MainWindow() override = default;

 private:
    void prep_window();

    //-----------------------------Events:-----------------------------
    static void on_inspector_clicked();
    void on_toggle_full_screen_clicked();
    void on_window_state_changed();
    bool on_key_pressed(guint keyVal, guint keyCode, Gdk::ModifierType modifier);
    void on_full_screen_changed();
};
}  // namespace ui::windows

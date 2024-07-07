#pragma once

#include "ui/widgets/core/ConnectionStatusWidget.hpp"
#include "ui/widgets/rpm/RepoListWidget.hpp"
#include "ui/widgets/rpm/RpmListWidget.hpp"
#include <gdkmm/enums.h>
#include <glib/gtypes.h>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/stack.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/window.h>

namespace ui::windows {
class MainWindow : public Gtk::Window {
 private:
    GtkWidget* splitView{nullptr};
    Gtk::Button splitViewBtn{};
    Gtk::MenuButton headerMoreBtn{};
    Gtk::StackSidebar sidebar{};
    Gtk::Stack stack{};

    widgets::core::ConnectionStatusWidget connectionStatusWidget{};
    widgets::rpm::RpmListWidget rpmListWidget{};
    widgets::rpm::RepoListWidget repoListWidget{};

 public:
    MainWindow();
    MainWindow(MainWindow&&) = delete;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    ~MainWindow() override = default;

 private:
    void prep_window();
    void prep_stack();
    void toggle_fullscreen();

    //-----------------------------Events:-----------------------------
    void on_toggle_split_view_clicked();
    void on_window_state_changed();
    bool on_key_pressed(guint keyVal, guint keyCode, Gdk::ModifierType modifier);
    void on_full_screen_changed();
    void on_menu_item_activated(const Glib::VariantBase& actionVariant, const Glib::ustring& actionName);
};
}  // namespace ui::windows

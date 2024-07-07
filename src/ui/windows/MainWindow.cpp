#include "MainWindow.hpp"
#include <adwaita.h>
#include <gdkmm/enums.h>
#include <giomm/menu.h>
#include <giomm/menuitem.h>
#include <giomm/menumodel.h>
#include <giomm/simpleactiongroup.h>
#include <glib/gtypes.h>
#include <glibmm/refptr.h>
#include <gtkmm.h>
#include <gtkmm/label.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/window.h>
#include <spdlog/spdlog.h>

namespace ui::windows {
MainWindow::MainWindow() {
    prep_window();
}

void MainWindow::prep_window() {
    set_title("Pulp UI");
    set_default_size(800, 480);

// Make the window look like a debug window for debug builds:
#ifdef DEBUG
    add_css_class("devel");
#endif  // DEBUG

    // Keyboard events:
    Glib::RefPtr<Gtk::EventControllerKey> controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(sigc::mem_fun(*this, &MainWindow::on_key_pressed), false);
    add_controller(controller);

    // Content:

    // Header bar:
    Gtk::HeaderBar* headerBar = Gtk::make_managed<Gtk::HeaderBar>();

    headerMoreBtn.set_icon_name("view-more");
    headerMoreBtn.set_tooltip_text("More");
    Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();

    menu->append("⚙️ Settings", "app.settings");
    menu->append("🐞 Inspector", "app.inspector");
    menu->append("↔️ Fullscreen", "app.fullscreen");
    menu->append("ℹ️ About", "app.about");
    headerMoreBtn.set_menu_model(menu);
    headerBar->pack_end(headerMoreBtn);

    // Menu item actions
    Glib::RefPtr<Gio::SimpleActionGroup> actionGroup = Gio::SimpleActionGroup::create();
    auto add_action = [&](const Glib::ustring& name) {
        Glib::RefPtr<Gio::SimpleAction> action = Gio::SimpleAction::create(name);
        action->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::on_menu_item_activated), name));
        actionGroup->add_action(action);
    };
    add_action("settings");
    add_action("inspector");
    add_action("fullscreen");
    add_action("about");
    insert_action_group("app", actionGroup);

    splitViewBtn.set_image_from_icon_name("dock-left-symbolic");
    splitViewBtn.set_tooltip_text("Show/Hide split view");
    splitViewBtn.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_toggle_split_view_clicked), false);
    headerBar->pack_start(splitViewBtn);

    sidebar.set_stack(stack);
    prep_stack();

    splitView = adw_overlay_split_view_new();
    AdwOverlaySplitView* splitVIewType = ADW_OVERLAY_SPLIT_VIEW(splitView);
    adw_overlay_split_view_set_sidebar(splitVIewType, GTK_WIDGET(sidebar.gobj()));
    adw_overlay_split_view_set_content(splitVIewType, GTK_WIDGET(stack.gobj()));
    set_child(*Glib::wrap(splitView));

    set_titlebar(*headerBar);
}

void MainWindow::prep_stack() {
    Glib::RefPtr<Gtk::StackPage> connectivityPage = stack.add(connectionStatusWidget, "connectivity", "Connectivity");
    connectivityPage->set_icon_name("cloud-outline-thin-symbolic");

    Glib::RefPtr<Gtk::StackPage> rpmRepoListPage = stack.add(repoListWidget, "rpm-repo-list", "RPM Repositories");
    rpmRepoListPage->set_icon_name("list-compact-symbolic");

    Glib::RefPtr<Gtk::StackPage> rpmListPage = stack.add(rpmListWidget, "rpm-packages-list", "RPM Packages");
    rpmListPage->set_icon_name("package-x-generic-symbolic");
}

void MainWindow::toggle_fullscreen() {
    if (property_fullscreened().get_value()) {
        unfullscreen();
        SPDLOG_INFO("Unfullscreen.");
    } else {
        fullscreen();
        SPDLOG_INFO("Fullscreen.");
    }
}

//-----------------------------Events:-----------------------------
void MainWindow::on_toggle_split_view_clicked() {
    AdwOverlaySplitView* splitVIewType = ADW_OVERLAY_SPLIT_VIEW(splitView);
    adw_overlay_split_view_set_collapsed(splitVIewType, !adw_overlay_split_view_get_collapsed(splitVIewType));
}

bool MainWindow::on_key_pressed(guint keyVal, guint /*keyCode*/, Gdk::ModifierType /*modifier*/) {
    if (keyVal == GDK_KEY_Escape && property_fullscreened().get_value()) {
        unfullscreen();
        return true;
    }
    if (keyVal == GDK_KEY_F11) {
        toggle_fullscreen();
        return true;
    }
    return false;
}

void MainWindow::on_menu_item_activated(const Glib::VariantBase& /*actionVariant*/, const Glib::ustring& actionName) {
    SPDLOG_DEBUG("Menu item action invoked: {0}", actionName.c_str());

    if (actionName == "settings") {
        // TODO: Show settings window
        return;
    }

    if (actionName == "inspector") {
        gtk_window_set_interactive_debugging(true);
        SPDLOG_INFO("Inspector shown.");
        return;
    }

    if (actionName == "fullscreen") {
        toggle_fullscreen();
        return;
    }

    if (actionName == "about") {
        // TODO: Show about window
        return;
    }
}
}  // namespace ui::windows

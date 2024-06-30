#include "MainWindow.hpp"
#include <gdkmm/enums.h>
#include <glib/gtypes.h>
#include <gtkmm.h>
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

    // inspectorBtn.set_label("🐞");
    // inspectorBtn.set_tooltip_text("Inspector");
    // inspectorBtn.signal_clicked().connect(&MainWindow::on_inspector_clicked);
    // headerBar->pack_end(inspectorBtn);

    set_titlebar(*headerBar);
}

//-----------------------------Events:-----------------------------
void MainWindow::on_inspector_clicked() {
    gtk_window_set_interactive_debugging(true);
}

void MainWindow::on_toggle_full_screen_clicked() {
    if (property_fullscreened().get_value()) {
        unfullscreen();
    } else {
        fullscreen();
    }
};

bool MainWindow::on_key_pressed(guint keyVal, guint /*keyCode*/, Gdk::ModifierType /*modifier*/) {
    if (keyVal == GDK_KEY_Escape && property_fullscreened().get_value()) {
        unfullscreen();
        maximize();
        return true;
    }
    if (keyVal == GDK_KEY_F11) {
        if (property_fullscreened().get_value()) {
            unfullscreen();
            maximize();
        } else {
            fullscreen();
        }
        return true;
    }
    return false;
}
}  // namespace ui::windows

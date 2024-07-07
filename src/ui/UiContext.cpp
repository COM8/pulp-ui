#include "UiContext.hpp"
#include "ui/utils/UiUtils.hpp"
#include <adwaita.h>
#include <gtkmm/icontheme.h>

namespace ui {
int UiContext::run(int argc, char** argv) {
    // Initialize Adwaita:
    adw_init();

    // Create the main GTK application:
    app = Gtk::Application::create("de.pulp-ui.cli");

    // Add icon paths:
    Gtk::IconTheme::get_for_display(Gdk::Display::get_default())->add_resource_path("/de/pulp-ui/cli/icons/scalable/actions");

    // CSS style class:
    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), get_css_provider(), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    app->signal_startup().connect([&] {
        add_main_window();
    });

    // The app will return once execution finished:
    return app->run(argc, argv);
}

void UiContext::add_main_window() {
    if (!mainWindow) {
        mainWindow = std::make_unique<windows::MainWindow>();
    }
    app->add_window(*mainWindow);
    mainWindow->show();
}
}  // namespace ui

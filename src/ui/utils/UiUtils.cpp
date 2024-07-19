#include "UiUtils.hpp"
#include <cstddef>
#include <string>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

namespace ui {
Glib::RefPtr<Gtk::CssProvider> get_css_provider() {
    static Glib::RefPtr<Gtk::CssProvider> cssProvider;
    if (!cssProvider) {
        cssProvider = Gtk::CssProvider::create();
        cssProvider->load_from_file(Gio::File::create_for_uri("resource:///de/pulp-ui/cli/theme.css"));
        SPDLOG_DEBUG("CSS-Provider initialized.");
    }
    return cssProvider;
}

Glib::RefPtr<Gdk::Pixbuf> scale_image(const Glib::RefPtr<Gdk::Pixbuf>& pixBuf, double factor) {
    int width = static_cast<int>(std::round(static_cast<double>(pixBuf->get_width()) * factor));
    int height = static_cast<int>(std::round(static_cast<double>(pixBuf->get_height()) * factor));
    return pixBuf->scale_simple(width, height, Gdk::InterpType::BILINEAR);
}

std::string to_unit_string(size_t byteCount) {
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
}  // namespace ui
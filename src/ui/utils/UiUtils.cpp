#include "UiUtils.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <functional>
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

void ltrim(std::string_view& s) {
    s.remove_prefix(std::distance(s.cbegin(), std::find_if(s.cbegin(), s.cend(), std::not_fn([](int c) { return std::isspace(c) != 0; }))));
}

void rtrim(std::string_view& s) {
    s.remove_suffix(std::distance(s.crbegin(), std::find_if(s.crbegin(), s.crend(), std::not_fn([](int c) { return std::isspace(c) != 0; }))));
}

std::string to_lower_clean(const std::string& s) {
    std::string tmp = s;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char c) {
        if (std::isspace(c)) {
            return ' ';
        }
        return static_cast<char>(std::tolower(c));
    });
    return tmp;
}

void trim(std::string_view& s) {
    ltrim(s);
    rtrim(s);
}
}  // namespace ui
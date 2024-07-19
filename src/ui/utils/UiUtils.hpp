#pragma once

#include <cstddef>
#include <string>
#include <gtkmm.h>

namespace ui {
Glib::RefPtr<Gtk::CssProvider> get_css_provider();
Glib::RefPtr<Gdk::Pixbuf> scale_image(const Glib::RefPtr<Gdk::Pixbuf>& pixBuf, double factor);
/**
 * Converts a given amount of bytes to next best fitting unit.
 * Examples:
 * 1024 Byte -> 1KB
 * 1536 Byte -> 1.5KB
 **/
std::string to_unit_string(size_t byteCount);
}  // namespace ui
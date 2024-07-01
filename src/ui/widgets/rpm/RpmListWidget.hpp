#pragma once

#include <gtkmm.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::rpm {
class RpmListWidget : public Gtk::ScrolledWindow {
 private:
 public:
    RpmListWidget();
    RpmListWidget(RpmListWidget&&) = default;
    RpmListWidget(const RpmListWidget&) = delete;
    RpmListWidget& operator=(RpmListWidget&&) = default;
    RpmListWidget& operator=(const RpmListWidget&) = delete;
    ~RpmListWidget() override = default;

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
};
}  // namespace ui::widgets::rpm
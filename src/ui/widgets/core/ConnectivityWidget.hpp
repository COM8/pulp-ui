#pragma once

#include <gtkmm.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::core {
class ConnectivityWidget : public Gtk::ScrolledWindow {
 private:
 public:
    ConnectivityWidget();
    ConnectivityWidget(ConnectivityWidget&&) = default;
    ConnectivityWidget(const ConnectivityWidget&) = delete;
    ConnectivityWidget& operator=(ConnectivityWidget&&) = default;
    ConnectivityWidget& operator=(const ConnectivityWidget&) = delete;
    ~ConnectivityWidget() override = default;

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
};
}  // namespace ui::widgets::core
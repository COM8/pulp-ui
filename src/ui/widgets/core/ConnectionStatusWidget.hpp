#pragma once

#include "backend/pulp/core/ConnectionStatus.hpp"
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::core {
class ConnectionStatusWidget : public Gtk::ScrolledWindow {
 private:
    backend::pulp::core::ConnectionStatus connectionStatus{};

    Gtk::Box mainBox{Gtk::Orientation::VERTICAL};
    Gtk::Box statusBox{Gtk::Orientation::HORIZONTAL};
    Gtk::DrawingArea statusCircle{};
    Gtk::Label statusLabel{};
    Gtk::Label statusDescriptionLabel{};

    backend::pulp::core::ConnectionStatus::Status status{backend::pulp::core::ConnectionStatus::Status::DISCONNECTED};

 public:
    ConnectionStatusWidget();
    ConnectionStatusWidget(ConnectionStatusWidget&&) = delete;
    ConnectionStatusWidget(const ConnectionStatusWidget&) = delete;

    ~ConnectionStatusWidget() override = default;

    ConnectionStatusWidget& operator=(ConnectionStatusWidget&&) = delete;
    ConnectionStatusWidget& operator=(const ConnectionStatusWidget&) = delete;

 private:
    void prep_widget();
    void update_status();

    //-----------------------------Events:-----------------------------
    void on_connection_status_changed(backend::pulp::core::ConnectionStatus::Status status);
    void on_status_circle_draw(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height);
};
}  // namespace ui::widgets::core
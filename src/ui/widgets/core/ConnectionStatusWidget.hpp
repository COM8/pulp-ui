#pragma once

#include "backend/pulp/core/ConnectionStatus.hpp"
#include <gtkmm.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::core {
class ConnectionStatusWidget : public Gtk::ScrolledWindow {
 private:
    backend::pulp::core::ConnectionStatus connectionStatus{};

 public:
    ConnectionStatusWidget();
    ConnectionStatusWidget(ConnectionStatusWidget&&) = delete;
    ConnectionStatusWidget(const ConnectionStatusWidget&) = delete;

    ~ConnectionStatusWidget() override = default;

    ConnectionStatusWidget& operator=(ConnectionStatusWidget&&) = delete;
    ConnectionStatusWidget& operator=(const ConnectionStatusWidget&) = delete;

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    void on_connection_status_changed(backend::pulp::core::ConnectionStatus::Status status);
    void on_connected(const backend::pulp::core::StatusResponse& statusResponse);
};
}  // namespace ui::widgets::core
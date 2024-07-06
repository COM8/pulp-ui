#pragma once

#include "backend/pulp/core/ConnectionStatus.hpp"
#include <gtkmm.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::core {
class ConnectivityWidget : public Gtk::ScrolledWindow {
 private:
    backend::pulp::core::ConnectionStatus connectionStatus{};

 public:
    ConnectivityWidget();
    ConnectivityWidget(ConnectivityWidget&&) = delete;
    ConnectivityWidget(const ConnectivityWidget&) = delete;

    ~ConnectivityWidget() override = default;

    ConnectivityWidget& operator=(ConnectivityWidget&&) = delete;
    ConnectivityWidget& operator=(const ConnectivityWidget&) = delete;

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    void on_connection_status_changed(backend::pulp::core::ConnectionStatus::Status status);
    void on_connected(const backend::pulp::core::StatusResponse& statusResponse);
};
}  // namespace ui::widgets::core
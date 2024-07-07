#include "ConnectionStatusWidget.hpp"
#include "backend/pulp/core/ConnectionStatus.hpp"
#include <gtkmm/scrolledwindow.h>
#include <spdlog/spdlog.h>

namespace ui::widgets::core {
ConnectionStatusWidget::ConnectionStatusWidget() {
    prep_widget();

    connectionStatus.statusChanged.append([this](backend::pulp::core::ConnectionStatus::Status status) { this->on_connection_status_changed(status); });
    connectionStatus.onConnected.append([this](const backend::pulp::core::StatusResponse& statusResponse) { this->on_connected(statusResponse); });
}

void ConnectionStatusWidget::prep_widget() {
}

//-----------------------------Events:-----------------------------

void ConnectionStatusWidget::on_connection_status_changed(backend::pulp::core::ConnectionStatus::Status status) {
    SPDLOG_INFO("New connection state: {0}", static_cast<size_t>(status));
}

void ConnectionStatusWidget::on_connected(const backend::pulp::core::StatusResponse& /*statusResponse*/) {
}
}  // namespace ui::widgets::core
#include "ConnectionStatusWidget.hpp"
#include "backend/pulp/core/ConnectionStatus.hpp"
#include <chrono>
#include <bits/chrono.h>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <gtkmm/enums.h>
#include <gtkmm/scrolledwindow.h>
#include <spdlog/spdlog.h>

namespace ui::widgets::core {
ConnectionStatusWidget::ConnectionStatusWidget() {
    prep_widget();

    connectionStatus.statusChanged.append([this](backend::pulp::core::ConnectionStatus::Status status) { this->on_connection_status_changed(status); });
}

void ConnectionStatusWidget::prep_widget() {
    mainBox.set_margin(10);
    set_child(mainBox);

    statusDescriptionLabel.set_markup("<span weight='bold'>Connection Status</span>");
    mainBox.append(statusDescriptionLabel);

    statusCircle.set_draw_func(sigc::mem_fun(*this, &ConnectionStatusWidget::on_status_circle_draw));
    statusCircle.set_content_height(20);
    statusCircle.set_content_width(20);
    statusCircle.set_valign(Gtk::Align::CENTER);
    statusBox.set_margin_top(5);
    statusBox.append(statusCircle);

    statusLabel.set_margin_start(10);
    statusLabel.set_valign(Gtk::Align::CENTER);
    statusBox.append(statusLabel);

    statusBox.set_halign(Gtk::Align::CENTER);
    mainBox.append(statusBox);

    update_status();
}

void ConnectionStatusWidget::update_status() {
    const std::chrono::system_clock::time_point lastSuccessfulConnection = connectionStatus.get_last_successful_connection();
    switch (status) {
        case backend::pulp::core::ConnectionStatus::Status::DISCONNECTED:
            statusLabel.set_text("Disconnected");
            if (lastSuccessfulConnection == std::chrono::system_clock::time_point::min()) {
                statusBox.set_tooltip_text("There wasn't yet a successful connection!");
            } else {
                statusBox.set_tooltip_text(fmt::format("Last successful connection: {:%T}", lastSuccessfulConnection));
            }
            break;

        case backend::pulp::core::ConnectionStatus::Status::CONNECTED:
            statusLabel.set_text("Connected");
            assert(lastSuccessfulConnection != std::chrono::system_clock::time_point::min());
            statusBox.set_tooltip_text(fmt::format("Last successful connection: {:%T}", lastSuccessfulConnection));
            break;

        case backend::pulp::core::ConnectionStatus::Status::FAILED:
            statusLabel.set_text("Failed");
            if (lastSuccessfulConnection == std::chrono::system_clock::time_point::min()) {
                statusBox.set_tooltip_text(fmt::format("There wasn't yet a successful connection!\nMessage: {}", *connectionStatus.get_last_error_message()));
            } else {
                statusBox.set_tooltip_text(fmt::format("Last successful connection: {:%T}\nMessage: {}", lastSuccessfulConnection, *connectionStatus.get_last_error_message()));
            }
            break;

        default:
            assert(false);  // should not happen
            break;
    }
}

//-----------------------------Events:-----------------------------

void ConnectionStatusWidget::on_connection_status_changed(backend::pulp::core::ConnectionStatus::Status status) {
    this->status = status;
    statusCircle.queue_draw();
    update_status();
}

void ConnectionStatusWidget::on_status_circle_draw(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height) {
    ctx->save();

    Gdk::RGBA color;
    switch (status) {
        case backend::pulp::core::ConnectionStatus::Status::DISCONNECTED:
            color.set_rgba(0.8, 0.8, 0.8);
            break;

        case backend::pulp::core::ConnectionStatus::Status::CONNECTED:
            color.set_rgba(0.0, 1.0, 0.0);
            break;

        case backend::pulp::core::ConnectionStatus::Status::FAILED:
            color.set_rgba(1.0, 0.0, 0.0);
            break;

        default:
            assert(false);  // should not happen
            break;
    }

    // Create a radial gradient
    auto gradient = Cairo::RadialGradient::create(width, height, 2, width, width, width);
    gradient->add_color_stop_rgba(0, color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
    gradient->add_color_stop_rgba(1, color.get_red() * 0.8, color.get_green() * 0.8, color.get_blue() * 0.8, color.get_alpha() * 0.8);

    ctx->set_source(gradient);
    ctx->arc(width / 2.0, height / 2.0, std::min(width, height) / 2.0, 0, 2 * M_PI);  // Draw circle with radius 10
    ctx->fill();

    ctx->restore();
}
}  // namespace ui::widgets::core
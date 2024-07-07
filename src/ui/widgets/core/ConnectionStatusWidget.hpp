#pragma once

#include "backend/pulp/core/ConnectionStatus.hpp"
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/listbox.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::core {
class ConnectionStatusWidget : public Gtk::ScrolledWindow {
 private:
    backend::pulp::core::ConnectionStatus connectionStatus{};

    Gtk::Box mainBox{Gtk::Orientation::VERTICAL};

    Gtk::ListBox connectionStatusListBox{};
    GtkWidget* connectionStatusGroup{nullptr};

    Gtk::Box statusBox{Gtk::Orientation::HORIZONTAL};
    Gtk::DrawingArea statusCircle{};
    Gtk::Label statusLabel{};
    Gtk::Label statusDescriptionLabel{};

    Gtk::ListBox versionListBox{};
    GtkWidget* versionGroup{nullptr};

    // Connection
    Gtk::ListBox connectionListBox{};
    GtkWidget* connectionGroup{nullptr};

    GtkWidget* redisConnectionRow{nullptr};
    GtkWidget* dbConnectionRow{nullptr};

    // Storage
    Gtk::ListBox storageListBox{};
    GtkWidget* storageGroup{nullptr};

    Gtk::Box storageBox{Gtk::Orientation::VERTICAL};
    Gtk::ProgressBar storageProfessBar{};
    Gtk::Label storageUsageLabel{};
    Gtk::Label storageHeaderLabel{};

    // Content
    Gtk::ListBox contentListBox{};
    GtkWidget* contentGroup{nullptr};

    GtkWidget* contentOriginRow{nullptr};
    GtkWidget* contentPathPrefixRow{nullptr};

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
    static std::string to_unit_string(size_t byteCount);

    //-----------------------------Events:-----------------------------
    void on_connection_status_changed(backend::pulp::core::ConnectionStatus::Status status);
    void on_status_circle_draw(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height);
};
}  // namespace ui::widgets::core
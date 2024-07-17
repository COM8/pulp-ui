#include "RpmListWidget.hpp"
#include "backend/pulp/rpm/PulpRpmHelper.hpp"
#include "backend/storage/Settings.hpp"
#include <gtkmm/scrolledwindow.h>
#include <spdlog/spdlog.h>

namespace ui::widgets::rpm {
RpmListWidget::RpmListWidget() {
    prep_widget();
}

void RpmListWidget::prep_widget() {
    const backend::storage::Settings* settings = backend::storage::get_settings_instance();
    (void) backend::pulp::rpm::get_packages(settings->data.pulp);
}

//-----------------------------Events:-----------------------------
}  // namespace ui::widgets::rpm
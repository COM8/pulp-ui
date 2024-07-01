#pragma once

#include <gtkmm.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets::rpm {
class RepoListWidget : public Gtk::ScrolledWindow {
 private:
 public:
    RepoListWidget();
    RepoListWidget(RepoListWidget&&) = default;
    RepoListWidget(const RepoListWidget&) = delete;
    RepoListWidget& operator=(RepoListWidget&&) = default;
    RepoListWidget& operator=(const RepoListWidget&) = delete;
    ~RepoListWidget() override = default;

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
};
}  // namespace ui::widgets::rpm
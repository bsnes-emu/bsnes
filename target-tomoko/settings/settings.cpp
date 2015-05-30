#include "../tomoko.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "timing.cpp"
#include "advanced.cpp"
SettingsManager* settingsManager = nullptr;

SettingsManager::SettingsManager() {
  settingsManager = this;

  layout.setMargin(5);
  statusBar.setFont(Font::sans(8, "Bold"));

  setTitle("Configuration Settings");
  setSize({600, 400});
  setPlacement(0.0, 1.0);

  onSize([&] {
    input.mappingList.resizeColumns();
    hotkeys.mappingList.resizeColumns();
  });
}

auto SettingsManager::setVisible(bool visible) -> SettingsManager& {
  if(visible) {
    input.refreshMappings();
    hotkeys.refreshMappings();
  }
  Window::setVisible(visible);
  return *this;
}

auto SettingsManager::show(unsigned setting) -> void {
  panel.item(setting)->setSelected();
  setVisible();
  setFocused();
  doSize();
}

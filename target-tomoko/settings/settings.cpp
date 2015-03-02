#include "../tomoko.hpp"
#include "input.cpp"
SettingsManager* settingsManager = nullptr;

SettingsManager::SettingsManager() {
  settingsManager = this;

  layout.setMargin(5);
  statusBar.setFont(Font::sans(8, "Bold"));

  setTitle("Configuration Settings");
  setPosition({0, 0});
  setSize({600, 400});
}

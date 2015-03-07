#include "../tomoko.hpp"
#include "input.cpp"
#include "advanced.cpp"
SettingsManager* settingsManager = nullptr;

SettingsManager::SettingsManager() {
  settingsManager = this;

  layout.setMargin(5);
  statusBar.setFont(Font::sans(8, "Bold"));

  setTitle("Configuration Settings");
  setSize({600, 400});
  setPlacement(0.0, 1.0);

  input.mappingList.resizeColumns();
}

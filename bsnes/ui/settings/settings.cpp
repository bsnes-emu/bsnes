#include "../base.hpp"
#include "input.cpp"
SettingsWindow *settingsWindow = 0;

SettingsWindow::SettingsWindow() {
  setTitle("Configuration Settings");
  setGeometry({ 128, 128, 640, 360 });
  setStatusFont(application->boldFont);
  setStatusVisible();

  panelList.append("Input");

  inputSettings = new InputSettings;

  append(layout);
  layout.setMargin(5);
  layout.append(panelList, 120, ~0, 5);

  layout.append(*inputSettings, ~0, ~0);
}

SettingsWindow::~SettingsWindow() {
  delete inputSettings;
}

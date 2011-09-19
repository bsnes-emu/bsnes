#include "../base.hpp"
#include "input.cpp"
#include "advanced.cpp"
SettingsWindow *settingsWindow = 0;

SettingsWindow::SettingsWindow() {
  setTitle("Configuration Settings");
  setGeometry({ 128, 128, 640, 360 });
  setStatusVisible();
  windowManager->append(this, "SettingsWindow");

  panelList.append("Input");
  panelList.append("Advanced");

  inputSettings = new InputSettings;
  advancedSettings = new AdvancedSettings;

  append(layout);
  layout.setMargin(5);
  layout.append(panelList, 120, ~0, 5);

  panelList.onChange = [&] { setPanel(panelList.selection()); };

  setPanel(0);
}

SettingsWindow::~SettingsWindow() {
  delete advancedSettings;
  delete inputSettings;
}

void SettingsWindow::setPanel(unsigned n) {
  //TODO: removing layouts isn't working right, so for now we are hiding them on toggle

  layout.remove(*inputSettings);
  layout.remove(*advancedSettings);

  inputSettings->setVisible(false);
  advancedSettings->setVisible(false);

  switch(n) {
  case 0:
    layout.append(*inputSettings, ~0, ~0);
    inputSettings->setVisible();
    break;
  case 1:
    layout.append(*advancedSettings, ~0, ~0);
    advancedSettings->setVisible();
    break;
  }
}

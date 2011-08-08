#include "../base.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "advanced.cpp"
SettingsWindow settingsWindow;

void SettingsWindow::create() {
  setTitle("Configuration Settings");
  application.addWindow(this, "SettingsWindow", "160,160");
  setStatusFont(application.proportionalFontBold);
  setStatusVisible();

  panel.append("Video");
  panel.append("Audio");
  panel.append("Input");
  panel.append("Advanced");

  layout.setMargin(5);
  layout.append(panel, PanelWidth, ~0, 5);
  append(layout);
  setGeometry({ 0, 0, 640, 360 });

  videoSettings.create();
  audioSettings.create();
  inputSettings.create();
  advancedSettings.create();

  panel.onChange = { &SettingsWindow::change, this };
  panel.setSelection(2);
  change();

  onClose = [] { inputSettings.endAssignment(); };
}

void SettingsWindow::change() {
  unsigned selection = panel.selection();
  videoSettings.panelLayout.setVisible(selection == 0);
  audioSettings.panelLayout.setVisible(selection == 1);
  inputSettings.panelLayout.setVisible(selection == 2);
  advancedSettings.panelLayout.setVisible(selection == 3);
  if(selection == 2) inputSettings.focus();
}

#include "../base.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "advanced.cpp"
SettingsWindow *settingsWindow = 0;

void SettingsLayout::append(Sizable &sizable, unsigned width, unsigned height, unsigned spacing) {
  layout.append(sizable, width, height, spacing);
}

SettingsLayout::SettingsLayout() {
  setMargin(5);
  HorizontalLayout::append(spacer, 120, ~0, 5);
  HorizontalLayout::append(layout, ~0, ~0);
}

SettingsWindow::SettingsWindow() {
  setTitle("Configuration Settings");
  setGeometry({ 128, 128, 640, 360 });
  setStatusVisible();
  windowManager->append(this, "SettingsWindow");

  layout.setMargin(5);
  panelList.setFont(application->boldFont);
  panelList.append("Video");
  panelList.append("Audio");
  panelList.append("Input");
  panelList.append("Advanced");

  videoSettings = new VideoSettings;
  audioSettings = new AudioSettings;
  inputSettings = new InputSettings;
  advancedSettings = new AdvancedSettings;

  append(layout);
    layout.append(panelList, 120, ~0, 5);
  append(*videoSettings);
  append(*audioSettings);
  append(*inputSettings);
  append(*advancedSettings);

  panelList.onChange = [&] { setPanel(panelList.selection()); };

  setPanel(2);
}

SettingsWindow::~SettingsWindow() {
  delete advancedSettings;
  delete inputSettings;
  delete audioSettings;
  delete videoSettings;
}

void SettingsWindow::setPanel(unsigned n) {
  panelList.setSelection(n);

  videoSettings->setVisible(false);
  audioSettings->setVisible(false);
  inputSettings->setVisible(false);
  advancedSettings->setVisible(false);

  switch(n) {
  case 0: return videoSettings->setVisible();
  case 1: return audioSettings->setVisible();
  case 2: return inputSettings->setVisible();
  case 3: return advancedSettings->setVisible();
  }
}

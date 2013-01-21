#include "../ethos.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkey.cpp"
#include "timing.cpp"
#include "server.cpp"
#include "advanced.cpp"
Settings *settings = nullptr;

void SettingsLayout::append(Sizable &sizable, const Size &size, unsigned spacing) {
  layout.append(sizable, size, spacing);
}

SettingsLayout::SettingsLayout() {
  setMargin(5);
  HorizontalLayout::append(spacer, {120, ~0}, 5);
  HorizontalLayout::append(layout, { ~0, ~0});
}

Settings::Settings() {
  setGeometry({128, 128, 640, 360});
  windowManager->append(this, "Settings");

  setTitle("Configuration Settings");
  setStatusVisible();

  layout.setMargin(5);
  panelList.setFont(application->boldFont);
  panelList.append("Video");
  panelList.append("Audio");
  panelList.append("Input");
  panelList.append("Hotkeys");
  panelList.append("Timing");
  panelList.append("Server");
  panelList.append("Advanced");

  append(layout);
  layout.append(panelList, {120, ~0}, 5);
  append(*videoSettings);
  append(*audioSettings);
  append(*inputSettings);
  append(*hotkeySettings);
  append(*timingSettings);
  append(*serverSettings);
  append(*advancedSettings);

  onClose = [&] {
    timingSettings->analysis.stop = true;
  };

  panelList.onChange = {&Settings::panelChanged, this};

  panelList.setSelection(2);
  panelChanged();
}

void Settings::panelChanged() {
  setStatusText("");
  videoSettings->setVisible(false);
  audioSettings->setVisible(false);
  inputSettings->setVisible(false);
  hotkeySettings->setVisible(false);
  timingSettings->setVisible(false);
  serverSettings->setVisible(false);
  advancedSettings->setVisible(false);
  if(panelList.selected() == false) return;

  switch(panelList.selection()) {
  case 0: return videoSettings->setVisible();
  case 1: return audioSettings->setVisible();
  case 2: return inputSettings->setVisible();
  case 3: return hotkeySettings->setVisible();
  case 4: return timingSettings->setVisible();
  case 5: return serverSettings->setVisible();
  case 6: return advancedSettings->setVisible();
  }
}

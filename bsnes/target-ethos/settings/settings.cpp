#include "../ethos.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
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
  setTitle("Configuration Settings");
  setGeometry({128, 128, 640, 360});
  setStatusFont(application->boldFont);
  setStatusVisible();

  layout.setMargin(5);
  panelList.setFont(application->boldFont);
  panelList.append("Video");
  panelList.append("Audio");
  panelList.append("Input");

  append(layout);
  layout.append(panelList, {120, ~0}, 5);
  append(*videoSettings);
  append(*audioSettings);
  append(*inputSettings);

  panelList.onChange = {&Settings::panelChanged, this};

  panelList.setSelection(2);
  panelChanged();
}

void Settings::panelChanged() {
  videoSettings->setVisible(false);
  audioSettings->setVisible(false);
  inputSettings->setVisible(false);
  if(panelList.selected() == false) return;

  switch(panelList.selection()) {
  case 0: return videoSettings->setVisible();
  case 1: return audioSettings->setVisible();
  case 2: return inputSettings->setVisible();
  }
}

#include "../bsnes.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "paths.cpp"
#include "advanced.cpp"
Settings settings;
unique_pointer<SettingsWindow> settingsWindow;

Settings::Settings() {
  Markup::Node::operator=(BML::unserialize(string::read(locate("settings.bml"))));

  auto set = [&](string name, string value) {
    //create node and set to default value only if it does not already exist
    if(!operator[](name)) operator()(name).setValue(value);
  };

  set("Video/Driver", Video::safestDriver());
  set("Video/Exclusive", false);
  set("Video/Blocking", false);
  set("Video/Shader", "Blur");

  set("Audio/Driver", Audio::safestDriver());
  set("Audio/Exclusive", false);
  set("Audio/Blocking", true);
  set("Audio/Device", "");
  set("Audio/Frequency", 48000.0);
  set("Audio/Latency", 0);
  set("Audio/Mute", false);

  set("Input/Driver", Input::safestDriver());
  set("Input/Frequency", 5);
  set("Input/Defocus", "Pause");

  set("View/Size", "Small");
  set("View/AspectCorrection", true);
  set("View/OverscanCropping", true);
  set("View/IntegralScaling", true);
  set("View/BlurEmulation", true);
  set("View/ColorEmulation", true);

  set("Path/Games", "");
  set("Path/Patches", "");
  set("Path/Saves", "");
  set("Path/Cheats", "");
  set("Path/States", "");
  set("Path/Recent/SuperNintendo", Path::user());
  set("Path/Recent/GameBoy", Path::user());
  set("Path/Recent/BSMemory", Path::user());
  set("Path/Recent/SufamiTurbo", Path::user());

  set("UserInterface/ShowStatusBar", true);

  set("Emulator/FastPPU", true);
  set("Emulator/FastDSP", true);
  set("Emulator/AutoSaveMemory/Enable", true);
  set("Emulator/AutoSaveMemory/Interval", 30);

  set("Crashed", false);
}

auto Settings::save() -> void {
  file::write(locate("settings.bml"), BML::serialize(*this));
}

SettingsWindow::SettingsWindow() {
  settingsWindow = this;

  layout.setMargin(5);
  statusBar.setFont(Font().setBold());

  setTitle("Settings");
  setSize({600, 400});
  setAlignment({0.0, 1.0});
  setDismissable();

  onSize([&] {
    input.mappingList.resizeColumns();
    hotkeys.mappingList.resizeColumns();
  });
}

auto SettingsWindow::setVisible(bool visible) -> SettingsWindow& {
  if(visible) {
    input.refreshMappings();
    hotkeys.refreshMappings();
  }
  return Window::setVisible(visible), *this;
}

auto SettingsWindow::show(uint index) -> void {
  panel.item(index)->setSelected();
  setVisible();
  setFocused();
  doSize();
}

#include "../bsnes.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "paths.cpp"
#include "emulator.cpp"
#include "drivers.cpp"
Settings settings;
VideoSettings videoSettings;
AudioSettings audioSettings;
InputSettings inputSettings;
HotkeySettings hotkeySettings;
PathSettings pathSettings;
EmulatorSettings emulatorSettings;
DriverSettings driverSettings;
SettingsWindow settingsWindow;

Settings::Settings() {
  Markup::Node::operator=(BML::unserialize(string::read(locate("settings.bml"))));

  auto set = [&](string name, string value) {
    //create node and set to default value only if it does not already exist
    if(!operator[](name)) operator()(name).setValue(value);
  };

  set("Video/Driver", Video::safestDriver());
  set("Video/Exclusive", false);
  set("Video/Blocking", false);
  set("Video/Flush", false);
  set("Video/Format", "Default");
  set("Video/Shader", "Blur");
  set("Video/Luminance", "100%");
  set("Video/Saturation", "100%");
  set("Video/Gamma", "150%");

  set("Audio/Driver", Audio::safestDriver());
  set("Audio/Exclusive", false);
  set("Audio/Device", "");
  set("Audio/Blocking", true);
  set("Audio/Dynamic", false);
  set("Audio/Frequency", "48000hz");
  set("Audio/Latency", 0);
  set("Audio/Mute", false);
  set("Audio/Skew", "0");
  set("Audio/Volume", "100%");
  set("Audio/Balance", "50%");

  set("Input/Driver", Input::safestDriver());
  set("Input/Frequency", 5);
  set("Input/Defocus", "Pause");
  set("Input/Turbo/Frequency", 4);

  set("View/Multiplier", "2");
  set("View/Output", "Scale");
  set("View/AspectCorrection", true);
  set("View/OverscanCropping", true);
  set("View/BlurEmulation", true);

  set("Path/Games", "");
  set("Path/Patches", "");
  set("Path/Saves", "");
  set("Path/Cheats", "");
  set("Path/States", "");
  set("Path/Screenshots", "");
  set("Path/Recent/SuperFamicom", Path::user());
  set("Path/Recent/GameBoy", Path::user());
  set("Path/Recent/BSMemory", Path::user());
  set("Path/Recent/SufamiTurboA", Path::user());
  set("Path/Recent/SufamiTurboB", Path::user());

  set("UserInterface/ShowStatusBar", true);
  set("UserInterface/SuppressScreenSaver", true);

  set("Emulator/WarnOnUnverifiedGames", false);
  set("Emulator/AutoSaveMemory/Enable", true);
  set("Emulator/AutoSaveMemory/Interval", 30);
  set("Emulator/AutoSaveStateOnUnload", false);
  set("Emulator/AutoLoadStateOnLoad", false);
  set("Emulator/Hack/FastPPU", true);
  set("Emulator/Hack/FastPPU/NoSpriteLimit", false);
  set("Emulator/Hack/FastPPU/HiresMode7", false);
  set("Emulator/Hack/FastDSP", true);
  set("Emulator/Hack/FastSuperFX", "100%");
  set("Emulator/Cheats/Enable", true);

  set("Crashed", false);
}

auto Settings::save() -> void {
  file::write(locate("settings.bml"), BML::serialize(*this));
}

auto SettingsWindow::create() -> void {
  layout.setPadding(5);
  panel.append(videoSettings);
  panel.append(audioSettings);
  panel.append(inputSettings);
  panel.append(hotkeySettings);
  panel.append(pathSettings);
  panel.append(emulatorSettings);
  panel.append(driverSettings);
  statusBar.setFont(Font().setBold());

  setTitle("Settings");
  setSize({600, 400});
  setAlignment({0.0, 1.0});
  setDismissable();

  onSize([&] {
    inputSettings.mappingList.resizeColumns();
    hotkeySettings.mappingList.resizeColumns();
  });

  onClose([&] {
    if(inputSettings.activeMapping) inputSettings.cancelMapping();
    if(hotkeySettings.activeMapping) hotkeySettings.cancelMapping();
    setVisible(false);
  });
}

auto SettingsWindow::setVisible(bool visible) -> SettingsWindow& {
  if(visible) {
    inputSettings.refreshMappings();
    hotkeySettings.refreshMappings();
  }
  return Window::setVisible(visible), *this;
}

auto SettingsWindow::show(uint index) -> void {
  panel.item(index)->setSelected();
  setVisible();
  setFocused();
  doSize();
}

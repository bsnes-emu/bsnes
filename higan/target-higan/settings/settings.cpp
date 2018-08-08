#include "../higan.hpp"
#include "system-properties.cpp"
#include "systems.cpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "advanced.cpp"
Settings settings;
unique_pointer<SettingsManager> settingsManager;
unique_pointer<SystemProperties> systemProperties;

Settings::Settings() {
  Markup::Node::operator=(BML::unserialize(string::read(locate("settings.bml")), " "));

  auto set = [&](const string& name, const string& value) {
    //create node and set to default value only if it does not already exist
    if(!operator[](name)) operator()(name).setValue(value);
  };

  set("Library/Location", {Path::user(), "Emulation/"});
  set("Library/IgnoreManifests", false);

  set("Video/Driver", ruby::Video::safestDriver());
  set("Video/Exclusive", false);
  set("Video/Synchronize", false);
  set("Video/Shader", "Blur");
  set("Video/BlurEmulation", true);
  set("Video/ColorEmulation", true);
  set("Video/ScanlineEmulation", false);

  set("Video/Saturation", 100);
  set("Video/Gamma", 100);
  set("Video/Luminance", 100);

  set("Audio/Driver", ruby::Audio::safestDriver());
  set("Audio/Device", "");
  set("Audio/Frequency", 48000);
  set("Audio/Latency", 0);
  set("Audio/Exclusive", false);
  set("Audio/Synchronize", true);
  set("Audio/Mute", false);
  set("Audio/Volume", 100);
  set("Audio/Balance", 50);

  set("Input/Driver", ruby::Input::safestDriver());
  set("Input/Frequency", 5);
  set("Input/Defocus", "Pause");

  set("View/Multiplier", "2");
  set("View/Output", "Scale");
  set("View/Adaptive", false);
  set("View/AspectCorrection", true);
  set("View/Overscan", true);
  set("View/Overscan/Horizontal", 0);
  set("View/Overscan/Vertical", 8);
  set("View/StatusBar", true);

  set("Emulation/AutoSaveMemory/Enable", true);
  set("Emulation/AutoSaveMemory/Interval", 30);

  set("Systems", "");

  set("Crashed", false);
}

auto Settings::save() -> void {
  file::write(locate("settings.bml"), BML::serialize(*this, " "));
}

//

SettingsManager::SettingsManager() {
  settingsManager = this;

  layout.setPadding(5);
  statusBar.setFont(Font().setBold());

  setTitle("Settings");
  setSize({600, 405});
  setAlignment({0.0, 1.0});
  setDismissable();

  onSize([&] {
    input.mappingList.resizeColumns();
    hotkeys.mappingList.resizeColumns();
  });
}

auto SettingsManager::setVisible(bool visible) -> SettingsManager& {
  if(visible) {
    input.refreshMappings();
    hotkeys.refreshMappings();
  }
  Window::setVisible(visible);
  return *this;
}

auto SettingsManager::show(uint setting) -> void {
  panel.item(setting)->setSelected();
  setVisible();
  setFocused();
  doSize();
}

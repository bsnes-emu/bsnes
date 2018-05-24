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
  Markup::Node::operator=(BML::unserialize(string::read(locate("settings.bml"))));

  auto set = [&](const string& name, const string& value) {
    //create node and set to default value only if it does not already exist
    if(!operator[](name)) operator()(name).setValue(value);
  };

  set("UserInterface/ShowStatusBar", true);

  set("Library/Location", {Path::user(), "Emulation/"});
  set("Library/IgnoreManifests", false);

  set("Video/Driver", ruby::Video::safestDriver());
  set("Video/Synchronize", false);
  set("Video/Shader", "Blur");
  set("Video/BlurEmulation", true);
  set("Video/ColorEmulation", true);
  set("Video/ScanlineEmulation", false);

  set("Video/Saturation", 100);
  set("Video/Gamma", 100);
  set("Video/Luminance", 100);

  set("Video/Overscan/Horizontal", 0);
  set("Video/Overscan/Vertical", 0);

  set("Video/Windowed/AspectCorrection", true);
  set("Video/Windowed/IntegralScaling", true);
  set("Video/Windowed/Adaptive", true);
  set("Video/Windowed/Scale", "Small");
  set("Video/Windowed/Scale/Small", "640x480");
  set("Video/Windowed/Scale/Medium", "960x720");
  set("Video/Windowed/Scale/Large", "1280x960");

  set("Video/Fullscreen/AspectCorrection", true);
  set("Video/Fullscreen/IntegralScaling", true);
  set("Video/Fullscreen/Exclusive", false);

  set("Audio/Driver", ruby::Audio::safestDriver());
  set("Audio/Device", "");
  set("Audio/Frequency", 48000);
  set("Audio/Latency", 0);
  set("Audio/Exclusive", false);
  set("Audio/Synchronize", true);
  set("Audio/Mute", false);
  set("Audio/Volume", 100);
  set("Audio/Balance", 50);
  set("Audio/Reverb/Enable", false);

  set("Input/Driver", ruby::Input::safestDriver());
  set("Input/Frequency", 5);
  set("Input/Defocus", "Pause");

  set("Emulation/AutoSaveMemory/Enable", true);
  set("Emulation/AutoSaveMemory/Interval", 30);

  set("Systems", "");

  set("Crashed", false);
}

auto Settings::save() -> void {
  file::write(locate("settings.bml"), BML::serialize(*this));
}

//

SettingsManager::SettingsManager() {
  settingsManager = this;

  layout.setMargin(5);
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

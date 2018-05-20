#include "../bsnes.hpp"
Settings settings;
unique_pointer<SettingsWindow> settingsWindow;

Settings::Settings() {
  Markup::Node::operator=(BML::unserialize(string::read(locate("settings.bml"))));

  auto set = [&](string name, string value) {
    //create node and set to default value only if it does not already exist
    if(!operator[](name)) operator()(name).setValue(value);
  };

  set("Video/Driver", Video::safestDriver());
  set("Video/Blocking", false);

  set("Audio/Driver", Audio::safestDriver());
  set("Audio/Blocking", true);
  set("Audio/Device", "");
  set("Audio/Frequency", 48000.0);
  set("Audio/Latency", 0);
  set("Audio/Mute", false);

  set("Input/Driver", Input::safestDriver());

  set("Path/Recent/SuperNintendo", Path::user());
  set("Path/Recent/GameBoy", Path::user());
  set("Path/Recent/BSMemory", Path::user());
  set("Path/Recent/SufamiTurbo", Path::user());

  set("Crashed", false);
}

auto Settings::save() -> void {
  file::write(locate("settings.bml"), BML::serialize(*this));
}

SettingsWindow::SettingsWindow() {
  settingsWindow = this;

  layout.setMargin(5);

  setTitle("Settings");
  setSize({600, 400});
  setAlignment({0.0, 1.0});
  setDismissable();
}

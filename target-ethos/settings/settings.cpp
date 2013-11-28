#include "../ethos.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkey.cpp"
#include "timing.cpp"
#include "server.cpp"
#include "advanced.cpp"
Settings* settings = nullptr;

SettingsLayout::SettingsLayout() {
  setMargin(5);
}

Settings::Settings() {
  setGeometry({128, 128, 640, 360});
  windowManager->append(this, "Settings");

  setTitle("Configuration Settings");
  setStatusVisible();

  layout.setMargin(5);
  panels.append("Video", {resource::video, sizeof resource::video});
  panels.append("Audio", {resource::audio, sizeof resource::audio});
  panels.append("Input", {resource::input, sizeof resource::input});
  panels.append("Hotkeys", {resource::hotkeys, sizeof resource::hotkeys});
  panels.append("Timing", {resource::timing, sizeof resource::timing});
  panels.append("Server", {resource::server, sizeof resource::server});
  panels.append("Advanced", {resource::advanced, sizeof resource::advanced});
  panels.setLayout(0, *videoSettings);
  panels.setLayout(1, *audioSettings);
  panels.setLayout(2, *inputSettings);
  panels.setLayout(3, *hotkeySettings);
  panels.setLayout(4, *timingSettings);
  panels.setLayout(5, *serverSettings);
  panels.setLayout(6, *advancedSettings);
  panels.setSelection(2);

  append(layout);
  layout.append(panels, {~0, ~0});

  onClose = [&] {
    timingSettings->analysis.stop = true;
    setVisible(false);
  };
}

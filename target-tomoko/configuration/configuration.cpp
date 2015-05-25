#include "../tomoko.hpp"
ConfigurationManager* configurationManager = nullptr;
auto config() -> ConfigurationManager& { return *configurationManager; }

ConfigurationManager::ConfigurationManager() {
  configurationManager = this;

  userInterface.append(userInterface.showStatusBar, "ShowStatusBar");
  append(userInterface, "UserInterface");

  library.append(library.location, "Location");
  append(library, "Library");

  video.append(video.driver, "Driver");
  video.append(video.synchronize, "Synchronize");
  video.append(video.scale, "Scale");
  video.append(video.aspectCorrection, "AspectCorrection");
  video.append(video.filter, "Filter");
  video.append(video.colorEmulation, "ColorEmulation");
  video.overscan.append(video.overscan.mask, "Mask");
  video.overscan.append(video.overscan.horizontal, "Horizontal");
  video.overscan.append(video.overscan.vertical, "Vertical");
  video.append(video.overscan, "Overscan");
  append(video, "Video");

  audio.append(audio.driver, "Driver");
  audio.append(audio.device, "Device");
  audio.append(audio.synchronize, "Synchronize");
  audio.append(audio.mute, "Mute");
  append(audio, "Audio");

  input.append(input.driver, "Driver");
  append(input, "Input");

  load({configpath(), "tomoko/settings.bml"});
  if(!library.location) library.location = {userpath(), "Emulation/"};
  if(!video.driver) video.driver = ruby::video.safestDriver();
  if(!audio.driver) audio.driver = ruby::audio.safestDriver();
  if(!input.driver) input.driver = ruby::input.safestDriver();
  save({configpath(), "tomoko/settings.bml"});
}

auto ConfigurationManager::quit() -> void {
  save({configpath(), "tomoko/settings.bml"});
}
